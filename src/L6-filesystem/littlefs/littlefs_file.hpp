#pragma once

namespace sys::littlefs {

struct file final : public sys::file
{
    public:

    ~file() override
    {
        if ( m_open )
        {
            close();
        }
    }

    [[nodiscard]] sys::error_code error_code() noexcept override
    {
        return m_error;
    };

    [[nodiscard]] bool is_open() noexcept override
    {
        return m_open && m_handle.get() != nullptr;
    };

    [[maybe_unused]] bool open(const std::filesystem::path& path, file_mode_type mode) noexcept override
    {
        if ( m_open || path.empty() )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        const auto ffmode = [&]() -> std::int32_t
        {
            switch ( mode )
            {
                case file_mode_type::k_read:
                    return LFS_O_RDONLY;

                case file_mode_type::k_read_write:
                    return LFS_O_RDWR;

                case file_mode_type::k_read_write_create_always:
                    return LFS_O_RDWR | LFS_O_CREAT;

                case file_mode_type::k_append:
                    return LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND;
            }

            return {};
        }();

        if ( ffmode == std::int32_t{} )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        if ( g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::owner_dead);
            return false;
        }

        if ( m_handle = std::make_unique<lfs_file_t>(); m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::not_enough_memory);
            return false;
        }

        if ( auto rc = lfs_file_open(g_littlefs_context, m_handle.get(), path.c_str(), ffmode); rc != lfs_error::LFS_ERR_OK )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            m_handle.reset(nullptr);
            return false;
        }

        m_open = true;

        return true;
    };

    [[maybe_unused]] bool close() noexcept override
    {
        if ( m_handle.get() != nullptr )
        {
            if ( g_littlefs_context == nullptr )
            {
                m_error = sys::make_error_code(std::errc::owner_dead);
                return false;
            }

            if ( auto rc = lfs_file_close(g_littlefs_context, m_handle.get()); rc != lfs_error::LFS_ERR_OK )
            {
                m_error = make_error_code(static_cast<lfs_error>(rc));
                return false;
            }

            m_handle.reset(nullptr);
        }

        m_open = false;

        return true;
    };

    [[maybe_unused]] std::size_t write(const std::span<const std::byte> buffer) noexcept override
    {
        if ( buffer.empty() )
            return {};

        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        if ( g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::owner_dead);
            return false;
        }

        auto byte_written_or_error = lfs_file_write(g_littlefs_context, m_handle.get(), buffer.data(), static_cast<lfs_size_t>(buffer.size_bytes()));
        if ( byte_written_or_error < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(byte_written_or_error));
            return {};
        }

        return static_cast<std::size_t>(byte_written_or_error);
    };

    [[maybe_unused]] std::size_t read(std::span<std::byte> buffer) noexcept override
    {
        if ( buffer.empty() )
            return {};

        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        if ( g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::owner_dead);
            return false;
        }

        auto byte_read_or_error = lfs_file_read(g_littlefs_context, m_handle.get(), buffer.data(), static_cast<lfs_size_t>(buffer.size_bytes()));
        if ( byte_read_or_error < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(byte_read_or_error));
            return {};
        }

        return static_cast<std::size_t>(byte_read_or_error);
    };

    [[nodiscard]] std::size_t tell() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        if ( g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::owner_dead);
            return {};
        }

        auto position_or_error = lfs_file_tell(g_littlefs_context, m_handle.get());
        if ( position_or_error < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(position_or_error));
            return {};
        }

        return static_cast<std::size_t>(position_or_error);
    };

    [[maybe_unused]] std::size_t seek(std::size_t offset, file_seek_origin_type origin) noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        if ( g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::owner_dead);
            return {};
        }

        const auto flag = [=]() -> lfs_whence_flags
        {
            switch ( origin )
            {
                case file_seek_origin_type::k_set:     return LFS_SEEK_SET;
                case file_seek_origin_type::k_current: return LFS_SEEK_CUR;
                case file_seek_origin_type::k_end:     return LFS_SEEK_END;
            }

            return {};
        }();

        auto new_position_or_error = lfs_file_seek(g_littlefs_context, m_handle.get(), static_cast<lfs_soff_t>(offset), flag);
        if ( new_position_or_error < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(new_position_or_error));
            return {};
        }

        return static_cast<std::size_t>(new_position_or_error);
    };

    [[nodiscard]] std::size_t size() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        if ( g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::owner_dead);
            return {};
        }

        auto size_or_error = lfs_file_size(g_littlefs_context, m_handle.get());
        if ( size_or_error < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(size_or_error));
            return {};
        }

        return static_cast<std::size_t>(size_or_error);
    };

    private:

    sys::error_code             m_error{};
    std::unique_ptr<lfs_file_t> m_handle{};
    bool                        m_open{false};
};

} // namespace sys::littlefs

