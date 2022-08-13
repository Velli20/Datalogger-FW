#pragma once

namespace sys::io {

struct stdio_stream_sink final : public io::output_sink_interface
{
    constexpr explicit stdio_stream_sink(bool enabled) noexcept
        : m_enabled{enabled}
        {}

    constexpr  stdio_stream_sink()                         noexcept = default;
    constexpr  stdio_stream_sink(stdio_stream_sink const&) noexcept = default;
    constexpr  stdio_stream_sink(stdio_stream_sink&&)      noexcept = default;
    constexpr ~stdio_stream_sink()                         noexcept = default;

    [[maybe_unused]] constexpr auto operator=(stdio_stream_sink&&)      noexcept -> stdio_stream_sink& = default;

    [[maybe_unused]] constexpr auto operator=(const stdio_stream_sink&) noexcept -> stdio_stream_sink& = default;

    [[maybe_unused]] auto operator<<(std::span<const std::byte> buffer) -> stdio_stream_sink& override
    {
        if ( m_enabled == true && buffer.empty() == false )
        {
            platform::stdio::write(buffer);
        }

        return *this;
    }

    [[maybe_unused]] auto operator<<(std::byte b) -> stdio_stream_sink& override
    {
        if ( m_enabled == false )
        {
            return *this;
        }

        platform::stdio::write(std::span{std::addressof(b), 1});

        return *this;
    }

    [[maybe_unused]] auto operator>>(std::span<std::byte> buffer) -> stdio_stream_sink&
    {
        if ( m_enabled == true && buffer.empty() == false )
        {
            platform::stdio::read(buffer);
        }

        return *this;
    }

    [[maybe_unused]] auto operator>>(std::byte& b) -> stdio_stream_sink&
    {
        if ( m_enabled == false )
        {
            b = {};
        }

        else
        {
            platform::stdio::read(std::span{std::addressof(b), 1});
        }

        return *this;
    }

    [[maybe_unused]] constexpr auto Enable(bool enable)
    {
        m_enabled = enable;
    }

    private:

    bool m_enabled{true};
};

} // namespace sys::io

