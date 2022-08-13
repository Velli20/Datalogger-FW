#pragma once

namespace sys::dev::device_tree {

static constexpr device_config k_device_tree[] =
{
    {
        "/dev/sensor/bme280@0", i2c_sensor_config_type
        {
            .bus     = "/dev/i2c@1",
            .address = 0x76
        }
    },

    {
        "/dev/serial@0", serial_console_config
        {
            .peripheral = "/chip/usb-otg",
            .m_baudrate = 230400
        }
    },

    {
        "/dev/serial@1", serial_console_config
        {
            .peripheral   = "/chip/usart@1",
            .m_baudrate    = 230400,
            .m_word_length = word_length_type::k_eight_data_bits,
            .m_stop_bits   = stop_bits_type::k_one,
            .m_parity      = parity_type::k_none
        }
    },

    {
        "/dev/serial@4", serial_console_config
        {
            .peripheral     = "/chip/uart@4",
            .m_baudrate     = 115200, // current-speed = <115200>
            .m_word_length  = word_length_type::k_eight_data_bits,
            .m_stop_bits    = stop_bits_type::k_one,
            .m_parity       = parity_type::k_none
        }
    },

    {
        "/dev/i2c@1", config::i2c_config_type
        {
            .peripheral = "/chip/i2c@1",
            .mode       = i2c_mode_type::k_master,
            .speed      = i2c_speed_type::k_standard,
            .addressing = i2c_addressing_type::k_seven_bit
        }
    },

    {
        "/dev/i2c@2", config::i2c_config_type
        {
            .peripheral = "/chip/i2c@2",
            .mode       =  i2c_mode_type::k_master,
            .speed      =  i2c_speed_type::k_standard,
            .addressing = i2c_addressing_type::k_seven_bit
        }
    },

    {
        "/dev/i2c@3", config::i2c_config_type
        {
            .peripheral = "/chip/i2c@3",
            .mode       = i2c_mode_type::k_master,
            .speed      = i2c_speed_type::k_standard,
            .addressing = i2c_addressing_type::k_seven_bit
        }
    },

    {
        "/dev/joystick@0", joystick_config_type
        {
            .pins =
            {
                { key_code_type::k_key_code_up,     device_node{"/chip/gpio-c@0,7"}  },
                { key_code_type::k_key_code_down,   device_node{"/chip/gpio-d@0,15"} },
                { key_code_type::k_key_code_left,   device_node{"/chip/gpio-d@0,13"} },
                { key_code_type::k_key_code_right,  device_node{"/chip/gpio-c@0,6"}  },
                { key_code_type::k_key_code_center, device_node{"/chip/gpio-d@0,14"} },
            }
        }
    },

    {
        "/dev/24-dc-input@1", high_voltage_input_config_type { "/chip/gpio-b@0,14" }
    },

    {
        "/dev/24-dc-input@2", high_voltage_input_config_type { "/chip/gpio-b@0,13" }
    },

    {
        "/dev/24-dc-input@3", high_voltage_input_config_type { "/chip/gpio-b@0,15" }
    },

    {
        "/dev/24-dc-input@4", high_voltage_input_config_type { "/chip/gpio-d@0,8" }
    },

    {
        "/dev/24-dc-input@5", high_voltage_input_config_type { "/chip/gpio-d@0,10" }
    },

    {
        "/dev/24-dc-input@6", high_voltage_input_config_type { "/chip/gpio-d@0,9" }
    },

    {
        "/dev/24-dc-input@7", high_voltage_input_config_type { "/chip/gpio-d@0,11" }
    },

    {
        "/dev/24-dc-input@8", high_voltage_input_config_type { "/chip/gpio-d@0,12" }
    },

    {
        "/dev/wifi@0", []() -> wifi_config_type
        {
            return
            {
                .uart        = "/dev/serial@4",
                .reset       = "/chip/gpio-b@0,2"  /* "/gpio/WiFi-Reset" */,
                .download    = "/chip/gpio-e@0,11" /* "/gpio/WiFi-Download" */,
                .wakeup      = "/chip/gpio-e@0,7"  /* "/gpio/WiFi-Wakeup" */,
                .cs          = "/chip/gpio-e@0,12" /* "/gpio/WiFi-SPI-CS" */,
                .sntp_server = "time.google.com"
            };
        }()
    },

    {
        "/dev/disk@0", memory_config_type<memory_type::k_sd>
        {
            .peripheral = "/chip/sdmmc@1"
        }
    },

    {
        "/dev/disk@1", memory_config_type<memory_type::k_nor_flash>
        {
            .peripheral = "/chip/octospi@1"
        }
    },

    {
        "/dev/led@1", led_config_type
        {
            .pin = "/chip/gpio-e@0,6" /* hal::hal::gpio_pin_of("LED1-Pin") */,
        }
    },

    {
        "/dev/led@2", led_config_type
        {
            .pin = "/chip/gpio-c@0,13" /* hal::hal::gpio_pin_of("LED2-Pin") */,
        }
    }
};

consteval auto get(const device_node node) -> auto&
{
    const auto is_node = [=](const auto& device) { return device.node == node; };

    const auto result = std::find_if(std::cbegin(k_device_tree), std::cend(k_device_tree), is_node);
    if ( result != std::cend(k_device_tree) )
    {
        return result->config;
    }
}

} // namespace sys::dev::device_tree
