#pragma once

// Includes.

#include <fatfs/ff.h>

namespace sys::fatfs {

struct filesystem final : public sys::filesystem
{
    private:

    static constexpr inline const char* k_root_path        = "";
    static constexpr inline std::size_t k_max_file_handles = 3;

    public:

    constexpr filesystem() = default;

    ~filesystem()
    {
        unmount();
    }

    [[maybe_unused]] bool format() noexcept override
    {
        const auto was_mounted = m_mounted.load();

        if ( was_mounted == true )
        {
            if ( auto success = unmount(); success == false )
            {
                return false;
            }
        }

        std::uint8_t work_buffer[_MAX_SS];

        if ( auto rc = f_mkfs("", FM_EXFAT, 0, work_buffer, sizeof(work_buffer)); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return false;
        }

        return was_mounted ? mount() : true;
    }

    [[maybe_unused]] bool mount() noexcept override
    {
        if ( auto is_mounted = m_mounted.exchange(true); is_mounted )
        {
            return true;
        }

        // Mount FatFs.

        if ( auto rc = f_mount(std::addressof(m_fatfs_context), k_root_path, 1); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return m_mounted.exchange(false) == false;
        }

        return true;
    }

    [[maybe_unused]] bool unmount() noexcept override
    {
        if ( auto is_mounted = m_mounted.exchange(false); is_mounted == false )
        {
            return true;
        }

        // Unmount FatFs.

        if ( auto rc = f_mount(nullptr, k_root_path, 0); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return m_mounted.exchange(true) == false;
        }

        return true;
    }

    [[maybe_unused]] bool create_directory(const std::filesystem::path& path) noexcept override
    {
        if ( m_mounted.load() == false )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return false;
        }

        if ( path.empty() )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        if ( auto rc = f_mkdir(path.c_str()); rc != FR_OK )
        {
            m_error = make_error_code(rc);
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

        if ( auto rc = f_unlink(path.c_str()); rc != FR_OK )
        {
            m_error = make_error_code(rc);
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

        if ( auto rc = f_rename(path.c_str(), new_path.c_str()); rc != FR_OK )
        {
            m_error = make_error_code(rc);
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
        if ( m_mounted.load() == false )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        std::filesystem::path tmp{m_path};

        tmp /= p;

        if ( auto rc = f_chdir(tmp.c_str()); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return {};
        }

        m_path = tmp;

        return m_path;
    }

    [[nodiscard]] file* open(const std::filesystem::path& path, file_mode_type mode) noexcept override
    {
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

    static inline std::array<file, k_max_file_handles> m_file_handles{};
    static inline FATFS                             m_fatfs_context{};
    static inline std::filesystem::path             m_path{k_root_path};
    static inline sys::error_code                   m_error{};
    static inline constinit std::atomic<bool>       m_mounted{false};
};

} // namespace sys::fatfs

