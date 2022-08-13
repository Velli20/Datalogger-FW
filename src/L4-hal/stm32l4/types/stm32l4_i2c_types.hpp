#pragma once

namespace sys::hal {

struct i2c_phy_config_type
{
    device_node scl_pin;
    device_node sda_pin;
};

} // namespace sys::hal
