#pragma once

namespace sys::dev {

struct readable_type
{
    using function_type  = bool (*)();

    constexpr auto operator()() const -> bool
    {
        return m_fun();
    }

    const function_type m_fun;
};

template <device_node kNodeName, typename T>
struct is_device
{
    static constexpr bool value = std::holds_alternative<T>(device_tree::get(kNodeName));
};

template <device_node kNodeName>
using is_i2c_device = is_device<kNodeName, config::i2c_config_type>;

template <device_node kNodeName>
using is_console_device = is_device<kNodeName, serial_console_config>;

template <device_node kNodeName>
using is_joystick_device = is_device<kNodeName, joystick_config_type>;

template <device_node kNodeName>
using is_high_voltage_channel_device = is_device<kNodeName, high_voltage_input_config_type>;

template <device_node kNodeName>
using is_led_device = is_device<kNodeName, led_config_type>;

template <device_node kNodeName>
using is_wifi_device = is_device<kNodeName, wifi_config_type>;

template <device_node kNodeName>
using is_flash_memory_device = is_device<kNodeName, memory_config_type<memory_type::k_nor_flash>>;

template <device_node kNodeName>
using is_sd_card_device = is_device<kNodeName, memory_config_type<memory_type::k_sd>>;


} // namespace sys::dev
