#pragma once

namespace sys::littlefs {

struct directory final : public sys::directory
{
    ~directory() override
    {
        if ( m_open == true )
        {
            close();
        }
    }

    [[maybe_unused]] bool rewind() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr || g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return false;
        }

        if ( auto rc = lfs_dir_rewind(g_littlefs_context, m_handle.get()); rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return false;
        }

        return true;
    }

    [[maybe_unused]] bool close() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr || g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return false;
        }

        if ( auto rc = lfs_dir_close(g_littlefs_context, m_handle.get()); rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return false;
        }

        m_open = false;

        m_handle.reset(nullptr);
        m_entry.reset(nullptr);

        return true;
    }

    [[maybe_unused]] bool open(const std::filesystem::path path) noexcept override
    {
        if ( g_littlefs_context == nullptr || m_open == true || g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        if ( m_handle = std::make_unique<lfs_dir_t>(); m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::not_enough_memory);
            return false;
        }

        if ( m_entry = std::make_unique<lfs_info>(); m_entry.get() == nullptr )
        {
            m_handle.reset(nullptr);

            m_error = sys::make_error_code(std::errc::not_enough_memory);
            return false;
        }

        // Try to open directory.

        if ( auto rc = lfs_dir_open(g_littlefs_context, m_handle.get(), path.c_str()); rc < 0 )
        {
            m_handle.reset(nullptr);
            m_entry.reset(nullptr);

            m_error = make_error_code(static_cast<lfs_error>(rc));
            return false;
        }

        m_open = true;

        return true;
    }

    [[nodiscard]] sys::error_code error_code() noexcept override
    {
        return m_error;
    }

    [[nodiscard]] directory_entry read() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr || g_littlefs_context == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        auto rc = lfs_dir_read(g_littlefs_context, m_handle.get(), m_entry.get());
        if ( rc < 0 )
        {
            m_error = make_error_code(static_cast<lfs_error>(rc));
            return {};
        }

        // If return code is 0, then we are at the end of directory.

        else if ( rc == 0 )
        {
            return {};
        }

        return directory_entry
        {
            std::string{m_entry.get()->name},
            (m_entry.get()->type == LFS_TYPE_DIR) ? directory_entry_type::k_directory : directory_entry_type::k_regular_file
        };
    }

    private:

    sys::error_code            m_error{};
    std::unique_ptr<lfs_dir_t> m_handle{};
    std::unique_ptr<lfs_info>  m_entry{};
    bool                       m_open{false};
};

} // namespace sys