#pragma once

namespace sys::littlefs {

struct filesystem final : public sys::filesystem
{
    private:

    static constexpr inline const char* k_root_path        = ""; // Must be empty.
    static constexpr inline std::size_t k_max_file_handles = 3;

    static constexpr inline std::size_t k_cache_size      = 256;
    static constexpr inline std::size_t k_look_ahead_size = 16;

    static inline std::uint32_t m_read_buffer[k_cache_size / sizeof(std::uint32_t)]{};
    static inline std::uint32_t m_prog_buffer[k_cache_size / sizeof(std::uint32_t)]{};
    static inline std::uint32_t m_lookahead_buffer[k_look_ahead_size/ sizeof(std::uint32_t)]{};

    static inline auto little_fs_read_region(
        const struct lfs_config* /* conf */,
        lfs_block_t              block,
        lfs_off_t                off,
        void*                    buffer,
        lfs_size_t               size
    ) -> int
    {
        using namespace sys::dev;

        auto address     = block * m_config.block_size + off;
        auto read_buffer = std::span{std::bit_cast<std::byte*>(buffer), static_cast<std::size_t>(size)};

        const auto read_count = device_type<"/dev/disk@1">::read(read_buffer, address);
        if ( read_count != static_cast<std::size_t>(size) )
        {
            return lfs_error::LFS_ERR_IO;
        }

        return {};
    }

    static inline auto little_fs_program_region(
        const struct lfs_config* /* conf */,
        lfs_block_t              block,
        lfs_off_t                off,
        const void*              buffer,
        lfs_size_t               size
    ) -> int
    {
        using namespace sys::dev;

        static_assert(std::is_same_v<lfs_off_t, std::uint32_t>, "");

        auto address      = block * m_config.block_size + off;
        auto write_buffer = std::span{std::bit_cast<const std::byte*>(buffer), static_cast<std::size_t>(size)};

        const auto write_count = device_type<"/dev/disk@1">::write(write_buffer, address);
        if ( write_count != static_cast<std::size_t>(size) )
        {
            return lfs_error::LFS_ERR_IO;
        }

        return {};
    }

    static inline auto little_fs_erase_region(
        const struct lfs_config* /* conf */,
        lfs_block_t              block
    ) -> int
    {
        using namespace sys::dev;

        auto success = device_type<"/dev/disk@1">::erase_sector(block);
        if ( success == false )
        {
            return lfs_error::LFS_ERR_IO;
        }

        return {};
    }

    static inline auto little_fs_sync(const struct lfs_config*) -> int
    {
        return {};
    }

    static inline auto little_fs_lock(const struct lfs_config*) -> int
    {
        return {};
    }

    static inline auto little_fs_unlock(const struct lfs_config*) -> int
    {
        return {};
    }

    static auto default_config() -> lfs_config
    {
        using namespace sys::dev;

        static_assert(sizeof(m_read_buffer)      == k_cache_size);
        static_assert(sizeof(m_prog_buffer)      == k_cache_size);
        static_assert(sizeof(m_lookahead_buffer) == k_look_ahead_size);

        return
        {
            .context          = nullptr,
            .read             = little_fs_read_region,
            .prog             = little_fs_program_region,
            .erase            = little_fs_erase_region,
            .sync             = little_fs_sync,
#ifdef LFS_THREADSAFE
            .lock             = little_fs_lock,
            .unlock           = little_fs_unlock,
#endif
            .read_size        = k_cache_size,
            .prog_size        = k_cache_size,
            .block_size       = 0,
            .block_count      = 0,
            .block_cycles     = -1,
            .cache_size       = k_cache_size,
            .lookahead_size   = k_look_ahead_size,
            .read_buffer      = m_read_buffer,
            .prog_buffer      = m_prog_buffer,
            .lookahead_buffer = m_lookahead_buffer,

            .name_max         = 0,
            .file_max         = 0,
            .attr_max         = 0,
            .metadata_max     = 0
        };
    }

    public:

    constexpr filesystem() = default;

    ~filesystem()
    {
        unmount();
    }

    [[maybe_unused]] bool format() noexcept override
    {
        if ( g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::io_error);
            return false;
        }

        const auto was_mounted = m_mounted.load();

        if ( was_mounted == true )
        {
            if ( auto success = unmount(); success == false )
            {
                m_error = sys::make_error_code(std::errc::io_error);
                return false;
            }
        }

        using namespace sys::dev;

        // Erase all sectors.

        if ( auto success = device_type<"/dev/disk@1">::erase_chip(); success == false )
        {
            return false;
        }

        if ( int rc = lfs_format(g_littlefs_context, std::addressof(m_config)); rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return false;
        }

        log::info() << "Formatted" << log::endl;

        return was_mounted ? mount() : true;
    }

    [[maybe_unused]] bool mount() noexcept override
    {
        if ( auto is_mounted = m_mounted.exchange(true); is_mounted )
            return true;

        using namespace sys::dev;

        // Initialize the flash disk.

        if ( device_type<"/dev/disk@1">::init() == false )
        {
            m_error = sys::make_error_code(std::errc::io_error);
            return m_mounted.exchange(false) == false;
        }

        m_config = littlefs::filesystem::default_config();

        {
            const auto result = device_type<"/dev/disk@1">::block_size();
            if ( result.has_value() == false )
            {
                m_error = sys::make_error_code(std::errc::io_error);
                return m_mounted.exchange(false) == false;
            }

            m_config.block_size = result.value();
        }

        {
            const auto result = device_type<"/dev/disk@1">::block_count();
            if ( result.has_value() == false )
            {
                m_error = sys::make_error_code(std::errc::io_error);
                return m_mounted.exchange(false) == false;
            }

            m_config.block_count = result.value();
        }

        if ( g_littlefs_context = new lfs_t{}; g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::not_enough_memory);
            return m_mounted.exchange(false) == false;
        }

        // Mount LittleFs.

        if ( auto rc = lfs_mount(g_littlefs_context, std::addressof(m_config)); rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return m_mounted.exchange(false) == false;
        }

        return true;
    }

    [[maybe_unused]] bool unmount() noexcept override
    {
        if ( auto is_mounted = m_mounted.exchange(false); is_mounted == false )
            return true;

        debug_assert(g_littlefs_context != nullptr);

        // Unmount LittleFs.

        if ( auto rc = lfs_unmount(g_littlefs_context); rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return not m_mounted.exchange(true);
        }

        return true;
    }

    [[maybe_unused]] bool create_directory(const std::filesystem::path& path) noexcept override
    {
        if ( path.empty() )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        if ( m_mounted.load() == false || g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::io_error);
            return false;
        }

        if ( auto rc = lfs_mkdir(g_littlefs_context, path.c_str()); rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return false;
        }

        return true;
    }

    [[maybe_unused]] bool unlink(const std::filesystem::path& path) noexcept override
    {
        if ( path.empty() )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        if ( m_mounted.load() == false || g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::io_error);
            return false;
        }

        if ( auto rc = lfs_remove(g_littlefs_context, path.c_str()); rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return false;
        }

        return true;
    }

    [[maybe_unused]] bool rename(const std::filesystem::path& path, const std::filesystem::path& new_path) noexcept override
    {
        if ( path.empty() || new_path.empty() )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        if ( m_mounted.load() == false || g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::io_error);
            return false;
        }

        if ( auto rc = lfs_rename(g_littlefs_context, path.c_str(), new_path.c_str()); rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return false;
        }

        return true;
    }

    [[nodiscard]] std::filesystem::path current_path() noexcept override
    {
        return m_path;
    }

    [[nodiscard]] std::filesystem::path current_path(const std::filesystem::path& p) noexcept override
    {
        if ( m_mounted.load() == false || g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::io_error);
            return {};
        }

        std::filesystem::path tmp{m_path};

        tmp /= p;

        // auto rc = f_chdir(tmp.c_str());
        // if ( rc != FR_OK )
        // {
        //     m_error = make_error_code(rc);
        //     return {};
        // }

        m_path = tmp;

        return m_path;
    }

    [[nodiscard]] file* open(const std::filesystem::path& path, file_mode_type mode) noexcept override
    {
        if ( m_mounted.load() == false )
        {
            m_error = sys::make_error_code(std::errc::io_error);
            return nullptr;
        }

        for ( auto& f : m_file_handles )
        {
            if ( f.is_open() == false )
            {
                f.open(path, mode);
                return std::addressof(f);
            }
        }

        m_error = sys::make_error_code(std::errc::too_many_files_open);

        return nullptr;
    }

    [[nodiscard]] sys::error_code error_code() noexcept override
    {
        return m_error;
    }

    private:

    static inline std::array<littlefs::file, k_max_file_handles> m_file_handles{};
    static inline std::filesystem::path                          m_path{k_root_path};
    static inline sys::error_code                                m_error{};
    static inline constinit std::atomic<bool>                    m_mounted{false};
    static inline lfs_config                                     m_config{};
};

} // namespace sys::littlefs

