#pragma once

namespace sys::hal {

template <device_node k_node>
struct peripheral_type {};

template <class T = std::monostate>
struct peripheral_config
{
    device_node   id;
    std::uint32_t reg{};
    T             config{};
};

} // namespace sys::hal
