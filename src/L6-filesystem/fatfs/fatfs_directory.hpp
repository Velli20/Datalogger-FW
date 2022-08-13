#pragma once

namespace sys::fatfs {

struct directory final : public sys::directory
{
    ~directory() override
    {
        if ( m_open )
        {
            close();
        }
    }

    [[maybe_unused]] bool rewind() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return false;
        }

        if ( auto rc = f_readdir(m_handle.get(), nullptr); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return false;
        }

        return true;
    }

    [[maybe_unused]] bool close() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return false;
        }

        if ( auto rc = f_closedir(m_handle.get()); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return false;
        }

        m_open = false;

        m_handle.reset(nullptr);
        m_entry.reset(nullptr);

        return true;
    }

    [[maybe_unused]] bool open(const std::filesystem::path path) noexcept override
    {
        if ( m_open )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        if ( m_handle = std::make_unique<DIR>(); m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::not_enough_memory);
            return false;
        }

        if ( m_entry = std::make_unique<FILINFO>(); m_entry.get() == nullptr )
        {
            m_entry.reset(nullptr);

            m_error = sys::make_error_code(std::errc::not_enough_memory);
            return false;
        }

        // Try to open a directory with given path.

        if ( auto rc = f_opendir(m_handle.get(), static_cast<const TCHAR*>(path.c_str())); rc != FR_OK )
        {
            m_entry.reset(nullptr);
            m_handle.reset(nullptr);

            m_error = make_error_code(rc);
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
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        if ( auto rc = f_readdir(m_handle.get(), m_entry.get()); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return {};
        }

        if ( m_entry.get()->fname == nullptr || m_entry.get()->fname[0] == '\0' )
        {
            return {};
        }

        return directory_entry
        {
            std::string{m_entry.get()->fname},
            (m_entry.get()->fattrib & AM_DIR) ? directory_entry_type::k_directory : directory_entry_type::k_regular_file
        };
    }

    private:

    sys::error_code          m_error{};
    std::unique_ptr<DIR>     m_handle{};
    std::unique_ptr<FILINFO> m_entry{};
    bool                     m_open{false};
};

} // namespace sys::fatfs