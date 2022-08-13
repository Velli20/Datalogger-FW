#pragma once

namespace sys::dev::concepts {

template <typename Driver>
concept display_driver = requires(Driver driver, std::span<const std::uint8_t> frame_buffer)
{
    { driver.refresh(frame_buffer) } -> std::same_as<bool>;
};

} // namespace sys::dev::concepts
