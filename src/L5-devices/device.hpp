#pragma once

namespace sys::dev {

struct device_config
{
    private:

    using configuration_type = std::variant<
        std::monostate,
        joystick_config_type,
        wifi_config_type,
        memory_config_type<memory_type::k_nor_flash>,
        memory_config_type<memory_type::k_sd>,
        led_config_type,
        config::i2c_config_type,
        serial_console_config,
        i2c_sensor_config_type,
        high_voltage_input_config_type
    >;

    public:

    device_node        node;
    configuration_type config;
};

} // namespace sys::dev
