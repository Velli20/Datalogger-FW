#pragma once

namespace sys::dev::config {

namespace detail {

template <typename T>
struct config {};

} // namespace detail

struct i2c_config_type
{
    device_node         peripheral{};
    i2c_mode_type       mode{};
    i2c_speed_type      speed{};
    i2c_addressing_type addressing{};
};

} // namespace sys::dev::config
