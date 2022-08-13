#pragma once

namespace sys::io {

struct output_sink_interface
{
    constexpr virtual ~output_sink_interface() noexcept = default;

    [[maybe_unused]] constexpr virtual auto operator<<(std::span<const std::byte> buffer) -> output_sink_interface& = 0;
    [[maybe_unused]] constexpr virtual auto operator<<(std::byte b)                       -> output_sink_interface& = 0;
};

} // namespace sys::io
