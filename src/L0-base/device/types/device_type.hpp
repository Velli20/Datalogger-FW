#pragma once

namespace sys::dev {

template <device_node k_node, typename Enable = void>
struct device_type {};

enum class key_code_type : std::uint32_t
{
    k_none = 0U,
    k_key_code_up,
    k_key_code_down,
    k_key_code_left,
    k_key_code_right,
    k_key_code_center,
};

enum class memory_type
{
    k_ram,
    k_sd,
    k_eeprom,
    k_nor_flash
};

struct joystick_config_type
{
    std::tuple<key_code_type, device_node> pins[5];
};

struct wifi_config_type
{
    device_node      uart;
    device_node      reset;
    device_node      download;
    device_node      wakeup;
    device_node      cs;
    std::string_view sntp_server;
};

template <memory_type k_type>
struct memory_config_type {};

template <>
struct memory_config_type<memory_type::k_nor_flash>
{
    device_node peripheral;
};

template <>
struct memory_config_type <memory_type::k_sd>
{
    device_node peripheral;
};

struct led_config_type
{
    device_node pin;
};

enum class stop_bits_type
{
    k_two,
    k_one
};

enum class parity_type
{
    k_none,
    k_even,
    k_odd
};

enum class word_length_type
{
    k_eight_data_bits,
    k_seven_data_bits,
    k_nine_data_bits
};

struct i2c_sensor_config_type
{
    device_node  bus;
    std::uint8_t address;
};

struct serial_console_config
{
    device_node      peripheral;
    std::uint32_t    m_baudrate{};
    word_length_type m_word_length{};
    stop_bits_type   m_stop_bits{};
    parity_type      m_parity{};
};

struct high_voltage_input_config_type
{
    device_node pin;
};


} // namespace sys::dev
