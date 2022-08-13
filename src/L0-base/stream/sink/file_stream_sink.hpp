#pragma once

namespace sys::io {

struct file_stream_sink final : public io::output_sink_interface
{
    explicit file_stream_sink(sys::file& f)
        : m_handle{f}
        {}

    constexpr file_stream_sink(file_stream_sink const&) noexcept = default;
    constexpr file_stream_sink(file_stream_sink&&)      noexcept = default;
    constexpr ~file_stream_sink()                       noexcept = default;

    [[maybe_unused]] auto operator=(file_stream_sink&&)      noexcept -> file_stream_sink& = default;
    [[maybe_unused]] auto operator=(const file_stream_sink&) noexcept -> file_stream_sink& = default;

    [[maybe_unused]] constexpr auto operator<<(std::span<const std::byte> buffer) -> file_stream_sink& override
    {
        if ( std::is_constant_evaluated() == false && buffer.empty() == false )
        {
            auto n = m_handle.get().write(buffer);
            debug_assert(n == buffer.size());
        }

        return *this;
    }

    [[maybe_unused]] constexpr auto operator<<(std::byte b) -> file_stream_sink& override
    {
        if ( std::is_constant_evaluated() == false )
        {
            auto n = m_handle.get().write(std::span{std::addressof(b), 1});
            debug_assert(n == 1);
        }

        return *this;
    }

    [[maybe_unused]] auto close()
    {
        return m_handle.get().close();
    }

    private:

    std::reference_wrapper<sys::file> m_handle;
};

} // namespace sys::log

