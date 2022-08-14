#pragma once

// Includes.

#include <ff.h>

namespace sys::fatfs {

struct file final : public sys::file
{
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

    [[maybe_unused]] bool open(const std::filesystem::path& path,
                               file_mode_type                    mode) noexcept override
    {
        if ( m_open || path.empty() )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        const auto ffmode = [&]() -> BYTE
        {
            switch ( mode )
            {
                case file_mode_type::k_read:
                    return FA_READ;

                case file_mode_type::k_read_write:
                    return FA_READ | FA_WRITE;

                case file_mode_type::k_read_write_create_always:
                    return FA_READ | FA_WRITE | FA_CREATE_ALWAYS;

                case file_mode_type::k_append:
                    return FA_READ | FA_WRITE | FA_OPEN_APPEND | FA_OPEN_ALWAYS;
            }

            return BYTE{};
        }();

        if ( ffmode == BYTE{} )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return false;
        }

        m_handle = std::make_unique<FIL>();
        if ( m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::not_enough_memory);
            return false;
        }

        if ( auto rc = f_open(m_handle.get(), path.c_str(), ffmode); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return false;
        }

        m_open = true;

        return true;
    };

    [[maybe_unused]] bool close() noexcept override
    {
        if ( m_handle.get() != nullptr )
        {
            if ( auto rc = f_close(m_handle.get()); rc != FR_OK )
            {
                m_error = make_error_code(rc);
                return false;
            }

            m_handle.reset(nullptr);
        }

        m_open = false;

        return true;
    };

    [[maybe_unused]] std::size_t write(const std::span<const std::byte> buffer) noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        if ( buffer.empty() )
            return {};

        UINT bw{};

        auto rc = f_write(m_handle.get(), buffer.data(), static_cast<UINT>(buffer.size_bytes()), std::addressof(bw));
        if ( rc != FR_OK )
        {
            m_error = make_error_code(rc);
        }

        static_assert(std::is_same_v<decltype(bw), std::size_t>, "");

        return bw;
    };

    [[maybe_unused]] std::size_t read(std::span<std::byte> buffer) noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        if ( buffer.size() == 0 )
        {
            m_error = sys::make_error_code(std::errc::invalid_argument);
            return {};
        }

        UINT br{};

        auto rc = f_read(m_handle.get(), buffer.data(), static_cast<UINT>(buffer.size_bytes()), std::addressof(br));
        if ( rc != FR_OK )
        {
            m_error = make_error_code(rc);
        }

        return static_cast<std::size_t>(br);
    };

    [[nodiscard]] std::size_t tell() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        return static_cast<std::size_t>(f_tell(m_handle.get()));
    };

    [[maybe_unused]] std::size_t seek(std::size_t offset, file_seek_origin_type origin) noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        auto position = [&]() -> FSIZE_t
        {
            switch ( origin )
            {
                case file_seek_origin_type::k_set:
                    return FSIZE_t{offset};

                case file_seek_origin_type::k_current:
                    return FSIZE_t{f_tell(m_handle.get()) + offset};

                case file_seek_origin_type::k_end:
                    return FSIZE_t{f_size(m_handle.get()) + offset};
            }

            debug_assert(false);
            return {};
        }();

        if ( auto rc = f_lseek(m_handle.get(), position); rc != FR_OK )
        {
            m_error = make_error_code(rc);
            return {};
        }

        return static_cast<std::size_t>(position);
    };

    [[nodiscard]] std::size_t size() noexcept override
    {
        if ( m_open == false || m_handle.get() == nullptr )
        {
            m_error = sys::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }

        return static_cast<std::size_t>(f_size(m_handle.get()));
    };

    private:

    sys::error_code      m_error{};
    std::unique_ptr<FIL> m_handle{};
    bool                 m_open{false};
};

} // namespace sys::fatfs

