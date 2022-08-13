#pragma once

// Includes.

#include "tusb.h"

namespace sys::dev {

template <>
struct device_type<"/dev/serial@0">
{
    private:

    constexpr device_type() = delete;

    static inline bool m_initialized{};

    public:

    static auto init() noexcept -> bool
    {
        m_initialized = true;

        return true;
    }

    static auto write(const std::span<const std::byte> buffer) noexcept -> std::size_t
    {
        if ( m_initialized == false || buffer.empty() )
        {
            return{};
        }

        std::size_t count{};

        for ( auto c : buffer )
        {
            std::size_t res = tud_cdc_write_char(std::to_integer<char>(c));

            if ( tud_cdc_write_available() == 0 )
            {
                tud_cdc_write_flush();
                if ( res != 0 )
                {
                    continue;
                }

                tud_cdc_write_char(std::to_integer<char>(c));
            }

            count += res;
        }

        tud_cdc_write_flush();

        return count;
    }

    static auto read(
        std::span<std::byte>      buffer,
        std::chrono::milliseconds timeout = std::chrono::milliseconds::max()
    ) noexcept -> std::size_t
    {
        if ( m_initialized == false || buffer.empty() )
        {
            return{};
        }

        std::size_t bytes_in{};

        while ( bytes_in < buffer.size() )
        {
            if ( tud_cdc_available() == 0 || tud_cdc_connected() == false )
            {
                if ( m_data_available_semaphore.try_acquire_for(timeout) == true )
                {
                    return bytes_in;
                }
            }

            buffer[bytes_in] = static_cast<std::byte>(tud_cdc_read_char());
            bytes_in        += 1;
        }

        return bytes_in;
    }

    static void on_data_available()
    {
        m_data_available_semaphore.release();
    }

    private:

    static inline platform::binary_semaphore m_data_available_semaphore{};
};

} // namespace sys::dev
