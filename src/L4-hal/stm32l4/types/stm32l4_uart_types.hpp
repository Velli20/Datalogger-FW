#pragma once

namespace sys::hal {

enum class uart_clock_prescaler_type : std::uint32_t
{
    k_none   = 0b0000,
    k_div2   = 0b0001,
    k_div4   = 0b0010,
    k_div6   = 0b0011,
    k_div8   = 0b0100,
    k_div10  = 0b0101,
    k_div16  = 0b0110,
    k_div32  = 0b0111,
    k_div64  = 0b1001,
    k_div128 = 0b1010,
    k_div256 = 0b1011,
};

enum class uart_mode_type : std::uint32_t
{
    k_receive_transmit = UART_MODE_TX_RX,
    k_receive          = UART_MODE_RX ,
    k_transmit         = UART_MODE_TX ,
};

enum class uart_oversampling_type : std::uint32_t
{
    k_by_sixteen = 0b0,
    k_by_eight   = 0b1
};

struct uart_phy_config_type
{
    rcc_clock_selection_type clocks{};
    uart_mode_type           mode;
    uart_oversampling_type   oversampling;
    device_node              tx_gpio;
    device_node              rx_gpio;
    device_node              dma_rx_channel;
    std::size_t              rx_buffer_size;
};

} // namespace sys::hal

namespace sys {

template <>
struct is_bitmask_enum <hal::uart_clock_prescaler_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::uart_mode_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::uart_oversampling_type> : std::true_type {};

} // namespace sys
