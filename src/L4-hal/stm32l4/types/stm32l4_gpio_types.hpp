#pragma once

namespace sys::hal {

enum class gpio_alternate_type : std::uint32_t
{
    k_usart1   = GPIO_AF7_USART1,
    k_usart2   = GPIO_AF7_USART2,
    k_usart3   = GPIO_AF7_USART3,
    k_uart4    = GPIO_AF8_UART4,
    k_uart5    = GPIO_AF8_UART5,
    k_sdmmc1   = GPIO_AF12_SDMMC1,
    k_spi2     = GPIO_AF5_SPI2,
    k_spi3     = GPIO_AF6_SPI3,
    k_i2c1     = GPIO_AF4_I2C1,
    k_i2c2     = GPIO_AF4_I2C2,
    k_i2c3     = GPIO_AF4_I2C3,
    k_usb_otg  = GPIO_AF10_OTG_FS,
    k_octospi1 = GPIO_AF10_OCTOSPIM_P1,
    k_none     = 0
};

enum class gpio_speed_type : std::uint32_t
{
    k_speed_low     = GPIO_SPEED_FREQ_LOW,
    k_speed_medium  = GPIO_SPEED_FREQ_MEDIUM,
    k_speed_high    = GPIO_SPEED_FREQ_HIGH,
    k_speed_highest = GPIO_SPEED_FREQ_VERY_HIGH,
};

enum class gpio_mode_type : std::uint32_t
{
    k_input                    = GPIO_MODE_INPUT,
    k_output_push_pull         = GPIO_MODE_OUTPUT_PP,
    k_output_open_drain        = GPIO_MODE_OUTPUT_OD,
    k_alternate_push_pull      = GPIO_MODE_AF_PP,
    k_alternate_open_drain     = GPIO_MODE_AF_OD,
    k_analog                   = GPIO_MODE_ANALOG,
    k_analog_adc               = GPIO_MODE_ANALOG_ADC_CONTROL,
    k_interrupt_rising         = GPIO_MODE_IT_RISING,
    k_interrupt_falling        = GPIO_MODE_IT_FALLING,
    k_interrupt_rising_falling = GPIO_MODE_IT_RISING_FALLING,
    k_event_rising             = GPIO_MODE_EVT_RISING,
    k_event_falling            = GPIO_MODE_EVT_FALLING,
    k_event_rising_falling     = GPIO_MODE_EVT_RISING_FALLING,
};

enum class gpio_pull_type : std::uint32_t
{
    k_none      = GPIO_NOPULL,
    k_pull_up   = GPIO_PULLUP,
    k_pull_down = GPIO_PULLDOWN,
};

struct gpio_phy_config_type
{
    gpio_mode_type      mode;
    gpio_pull_type      pull      = gpio_pull_type::k_none;
    gpio_alternate_type alternate = gpio_alternate_type::k_none;
    gpio_speed_type     speed     = gpio_speed_type::k_speed_low;
};

} // namespace sys::hal

namespace sys {

template <>
struct is_bitmask_enum <hal::gpio_alternate_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::gpio_speed_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::gpio_mode_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::gpio_pull_type> : std::true_type {};

} // namespace sys
