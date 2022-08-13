#pragma once

namespace sys::hal {

enum class dma_direction_type
{
    k_peripheral_to_memory,
    k_data_transfer_direction,
    k_memory_to_memory,
};

enum class dma_increment_type
{
    k_disable,
    k_enable,
};

enum class dma_data_size_type : std::uint32_t
{
    k_byte      = 0b00,
    k_half_word = 0b01,
    k_word      = 0b10,
};

enum class dma_mode_type : std::uint32_t
{
    k_normal   = 0b0,
    k_circular = 0b1
};

enum class dma_priority_type : std::uint32_t
{
    k_low     = 0b00,
    k_medium  = 0b01,
    k_high    = 0b10,
    k_highest = 0b11,
};

enum class dma_mux_request_type : std::uint32_t
{
    k_request_memory_to_memory      = 0U,
    k_request_generator0            = 1U,
    k_request_generator1            = 2U,
    k_request_generator2            = 3U,
    k_request_generator3            = 4U,
    k_request_adc1                  = 5U,
#if defined (STM32L4P5xx) || defined (STM32L4Q5xx)
    k_request_adc2                  =  6U,
    k_request_dac1_channel1         =  7U,
    k_request_dac1_channel2         =  8U,
    k_request_tim6_up               =  9U,
    k_request_tim7_up               = 10U,
    k_request_spi1_rx               = 11U,
    k_request_spi1_tx               = 12U,
    k_request_spi2_rx               = 13U,
    k_request_spi2_tx               = 14U,
    k_request_spi3_rx               = 15U,
    k_request_spi3_tx               = 16U,
    k_request_i2c1_rx               = 17U,
    k_request_i2c1_tx               = 18U,
    k_request_i2c2_rx               = 19U,
    k_request_i2c2_tx               = 20U,
    k_request_i2c3_rx               = 21U,
    k_request_i2c3_tx               = 22U,
    k_request_i2c4_rx               = 23U,
    k_request_i2c4_tx               = 24U,
    k_request_usart1_rx             = 25U,
    k_request_usart1_tx             = 26U,
    k_request_usart2_rx             = 27U,
    k_request_usart2_tx             = 28U,
    k_request_usart3_rx             = 29U,
    k_request_usart3_tx             = 30U,
    k_request_uart4_rx              = 31U,
    k_request_uart4_tx              = 32U,
    k_request_uart5_rx              = 33U,
    k_request_uart5_tx              = 34U,
    k_request_low_power_uart1_rx    = 35U,
    k_request_low_power_uart1_tx    = 36U,
    k_request_sai1_a                = 37U,
    k_request_sai1_b                = 38U,
    k_request_sai2_a                = 39U,
    k_request_sai2_b                = 40U,
    k_request_octo_spi1             = 41U,
    k_request_octo_spi2             = 42U,
    k_request_tim1_channel1         = 43U,
    k_request_tim1_channel2         = 44U,
    k_request_tim1_channel3         = 45U,
    k_request_tim1_channel4         = 46U,
    k_request_tim1_up               = 47U,
    k_request_tim1_trig             = 48U,
    k_request_tim1_com              = 49U,
    k_request_tim8_channel1         = 50U,
    k_request_tim8_channel2         = 51U,
    k_request_tim8_channel3         = 52U,
    k_request_tim8_channel4         = 53U,
    k_request_tim8_up               = 54U,
    k_request_tim8_trig             = 55U,
    k_request_tim8_com              = 56U,
    k_request_tim2_channel1         = 57U,
    k_request_tim2_channel2         = 58U,
    k_request_tim2_channel3         = 59U,
    k_request_tim2_channel4         = 60U,
    k_request_tim2_up               = 61U,
    k_request_tim3_channel1         = 62U,
    k_request_tim3_channel2         = 63U,
    k_request_tim3_channel3         = 64U,
    k_request_tim3_channel4         = 65U,
    k_request_tim3_up               = 66U,
    k_request_tim3_trig             = 67U,
    k_request_tim4_channel1         = 68U,
    k_request_tim4_channel2         = 69U,
    k_request_tim4_channel3         = 70U,
    k_request_tim4_channel4         = 71U,
    k_request_tim4_up               = 72U,
    k_request_tim5_channel1         = 73U,
    k_request_tim5_channel2         = 74U,
    k_request_tim5_channel3         = 75U,
    k_request_tim5_channel4         = 76U,
    k_request_tim5_up               = 77U,
    k_request_tim5_trig             = 78U,
    k_request_tim15_channel1        = 79U,
    k_request_tim15_up              = 80U,
    k_request_tim15_trig            = 81U,
    k_request_tim15_com             = 82U,
    k_request_tim16_channel1        = 83U,
    k_request_tim16_up              = 84U,
    k_request_tim17_channel1        = 85U,
    k_request_tim17_up              = 86U,
    k_request_dfsdm1_flt0           = 87U,
    k_request_dfsdm1_flt1           = 88U,
    k_request_dcmi                  = 91U,
    k_request_dcmi_pssi             = 91U,
    k_request_aes_in                = 92U,
    k_request_aes_out               = 93U,
    k_request_hash_in               = 94U,
#else
    k_request_dac1_channel1         =  6U,
    k_request_dac1_channel2         =  7U,
    k_request_tim6_up               =  8U,
    k_request_tim7_up               =  9U,
    k_request_spi1_rx               = 10U,
    k_request_spi1_tx               = 11U,
    k_request_spi2_rx               = 12U,
    k_request_spi2_tx               = 13U,
    k_request_spi3_rx               = 14U,
    k_request_spi3_tx               = 15U,
    k_request_i2c1_rx               = 16U,
    k_request_i2c1_tx               = 17U,
    k_request_i2c2_rx               = 18U,
    k_request_i2c2_tx               = 19U,
    k_request_i2c3_rx               = 20U,
    k_request_i2c3_tx               = 21U,
    k_request_i2c4_rx               = 22U,
    k_request_i2c4_tx               = 23U,
    k_request_usart1_rx             = 24U,
    k_request_usart1_tx             = 25U,
    k_request_usart2_rx             = 26U,
    k_request_usart2_tx             = 27U,
    k_request_usart3_rx             = 28U,
    k_request_usart3_tx             = 29U,
    k_request_uart4_rx              = 30U,
    k_request_uart4_tx              = 31U,
    k_request_uart5_rx              = 32U,
    k_request_uart5_tx              = 33U,
    k_request_low_power_uart1_rx    = 34U,
    k_request_low_power_uart1_tx    = 35U,
    k_request_sai1_a                = 36U,
    k_request_sai1_b                = 37U,
    k_request_sai2_a                = 38U,
    k_request_sai2_b                = 39U,
    k_request_octo_spi1             = 40U,
    k_request_octo_spi2             = 41U,
    k_request_tim1_channel1         = 42U,
    k_request_tim1_channel2         = 43U,
    k_request_tim1_channel3         = 44U,
    k_request_tim1_channel4         = 45U,
    k_request_tim1_up               = 46U,
    k_request_tim1_trig             = 47U,
    k_request_tim1_com              = 48U,
    k_request_tim8_channel1         = 49U,
    k_request_tim8_channel2         = 50U,
    k_request_tim8_channel3         = 51U,
    k_request_tim8_channel4         = 52U,
    k_request_tim8_up               = 53U,
    k_request_tim8_trig             = 54U,
    k_request_tim8_com              = 55U,
    k_request_tim2_channel1         = 56U,
    k_request_tim2_channel2         = 57U,
    k_request_tim2_channel3         = 58U,
    k_request_tim2_channel4         = 59U,
    k_request_tim2_up               = 60U,
    k_request_tim3_channel1         = 61U,
    k_request_tim3_channel2         = 62U,
    k_request_tim3_channel3         = 63U,
    k_request_tim3_channel4         = 64U,
    k_request_tim3_up               = 65U,
    k_request_tim3_trig             = 66U,
    k_request_tim4_channel1         = 67U,
    k_request_tim4_channel2         = 68U,
    k_request_tim4_channel3         = 69U,
    k_request_tim4_channel4         = 70U,
    k_request_tim4_up               = 71U,
    k_request_tim5_channel1         = 72U,
    k_request_tim5_channel2         = 73U,
    k_request_tim5_channel3         = 74U,
    k_request_tim5_channel4         = 75U,
    k_request_tim5_up               = 76U,
    k_request_tim5_trig             = 77U,
    k_request_tim15_channel1        = 78U,
    k_request_tim15_up              = 79U,
    k_request_tim15_trig            = 80U,
    k_request_tim15_com             = 81U,
    k_request_tim16_channel1        = 82U,
    k_request_tim16_up              = 83U,
    k_request_tim17_channel1        = 84U,
    k_request_tim17_up              = 85U,
    k_request_dfsdm1_flt0           = 86U,
    k_request_dfsdm1_flt1           = 87U,
    k_request_dfsdm1_flt2           = 88U,
    k_request_dfsdm1_flt3           = 89U,
    k_request_dcmi                  = 90U,
    k_request_aes_in                = 91U,
    k_request_aes_out               = 92U,
    k_request_hash_in               = 93U,
#endif
};

struct dma_channel_phy_config_type
{
    dma_data_size_type   data_size;
    dma_mode_type        mode;
    dma_increment_type   increment;
    dma_priority_type    priority;
    dma_mux_request_type request;
    dma_direction_type   direction;
};

} // namespace sys::hal

namespace sys {

template <>
struct is_bitmask_enum <hal::dma_data_size_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::dma_mode_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::dma_priority_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::dma_mux_request_type> : std::true_type {};

} // namespace sys
