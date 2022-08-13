#pragma once

namespace sys::hal {

using configuration_type = std::variant<
    std::monostate,
    gpio_phy_config_type,
    uart_phy_config_type,
    rtc_phy_config_type,
    sys_tick_config_type,
    sdmmc_phy_config_type,
    i2c_phy_config_type,
    usb_phy_config_type,
    dma_channel_phy_config_type,
    ospi_phy_config_type
>;

static constexpr peripheral_config<configuration_type> k_peripheral_tree[] =
{

    {
        .id  = "/chip/exti",
        .reg = APB2PERIPH_BASE + 0x0400UL,
    },

    {
        .id  = "/chip/dma-mux",
        .reg = AHB1PERIPH_BASE + 0x0800UL,
    },

    {
        .id  = "/chip/dma-mux-request-gen-status",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0140UL,
    },

    {
        .id  = "/chip/dma-mux-channel-status@0",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0080U,
    },

    {
        .id  = "/chip/dma-mux-channel@0",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0000UL,
    },

    {
        .id  = "/chip/dma-mux-channel@1",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0004UL,
    },

    {
        .id  = "/chip/dma-mux-channel@2",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0008UL,
    },

    {
        .id  = "/chip/dma-mux-channel@3",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'000CU,
    },

    {
        .id  = "/chip/dma-mux-channel@4",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0010UL,
    },

    {
        .id  = "/chip/dma-mux-channel@5",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0014UL,
    },

    {
        .id  = "/chip/dma-mux-channel@6",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0018UL,
    },

    {
        .id  = "/chip/dma-mux-channel@7",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'001CUL,
    },

    {
        .id  = "/chip/dma-mux-channel@8",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0020UL,
    },

    {
        .id  = "/chip/dma-mux-channel@9",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0024U,
    },

    {
        .id  = "/chip/dma-mux-channel@10",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0028UL,
    },

    {
        .id  = "/chip/dma-mux-channel@11",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'002CUL,
    },

    {
        .id  = "/chip/dma-mux-channel@12",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0030UL,
    },
    {
        .id  = "/chip/dma-mux-channel@13",
        .reg = AHB1PERIPH_BASE + 0x0800UL + 0x0000'0034UL,
    },

    {
        .id  = "/chip/dma@1",
        .reg = AHB1PERIPH_BASE + 0x0000UL,
    },

    {
        .id  = "/chip/dma@2",
        .reg = AHB1PERIPH_BASE + 0x0400UL,
    },

    {
        .id     = "/chip/dma1-channel@5",
        .reg    = AHB1PERIPH_BASE + 0x0000UL + 0x0058UL,
        .config = dma_channel_phy_config_type
        {
            .data_size = dma_data_size_type::k_byte,
            .mode      = dma_mode_type::k_circular,
            .increment = dma_increment_type::k_enable,
            .priority  = dma_priority_type::k_low,
            .request   = dma_mux_request_type::k_request_usart1_rx,
            .direction = dma_direction_type::k_peripheral_to_memory
        },
    },

    {
        .id     = "/chip/dma2-channel@1",
        .reg    = AHB1PERIPH_BASE + 0x0400UL + 0x0008UL,
        .config = dma_channel_phy_config_type
        {
            .data_size  = dma_data_size_type::k_byte,
            .mode       = dma_mode_type::k_circular,
            .increment  = dma_increment_type::k_enable,
            .priority   = dma_priority_type::k_low,
            .request    = dma_mux_request_type::k_request_uart4_rx,
            .direction  = dma_direction_type::k_peripheral_to_memory
        },
    },

    {
        .id     = "/chip/usart@1",
        .reg    = APB2PERIPH_BASE + 0x3800UL,
        .config = uart_phy_config_type
        {
            .clocks         = rcc_clock_selection_type::k_pclk,
            .mode           = uart_mode_type::k_receive_transmit,
            .oversampling   = uart_oversampling_type::k_by_sixteen,
            .tx_gpio        = "/chip/gpio-a@0,9",
            .rx_gpio        = "/chip/gpio-a@0,10",
            .dma_rx_channel = "/chip/dma1-channel@5",
            .rx_buffer_size = sys::units::literals::operator""_b(128),
        }
    },

    {
        .id     = "/chip/uart@4",
        .reg    = APB1PERIPH_BASE + 0x4C00UL,
        .config = uart_phy_config_type
        {
            .clocks         = rcc_clock_selection_type::k_pclk,
            .mode           = uart_mode_type::k_receive_transmit,
            .oversampling   = uart_oversampling_type::k_by_sixteen,
            .tx_gpio        = "/chip/gpio-a@0,0",
            .rx_gpio        = "/chip/gpio-a@0,1",
            .dma_rx_channel = "/chip/dma2-channel@1",
            .rx_buffer_size = sys::units::literals::operator""_b(2'048),
        }
    },

    {
        .id     = "/chip/rtc",
        .reg    = APB1PERIPH_BASE + 0x2800UL,
        .config = rtc_phy_config_type
        {
            .polarity             = rtc_output_polarity_type::k_pin_high,
            .output               = rtc_output_type::k_none,
            .format               = rtc_hour_format_type::k_24h,
            .synchronous_divider  = 0x00FF, /* For LSE @ 32768 Hz */
            .asynchronous_divider = 0x7F,
        }
    },

    {
        .id     = "/chip/cortex-m-systick",
        .reg    = SCS_BASE + 0x0010UL,
        .config = sys_tick_config_type
        {
            .source         = sys_tick_source_type::k_processor_clock,
            .tick_frequency = /* 1_kHz */ 1000,
        }
    },

    {
        .id  = "/chip/cortex-m-scb",
        .reg = SCS_BASE + 0x0010UL,
    },

    {
        .id  = "/chip/cortex-m-dwt",
        .reg = DWT_BASE,
    },

    {
        .id  = "/chip/cortex-m-debug",
        .reg = CoreDebug_BASE,
    },

    {
        .id  = "/chip/power",
        .reg = PWR_BASE,
    },

    {
        .id  = "/chip/system-configuration",
        .reg = APB2PERIPH_BASE + 0x0000UL,
    },

    {
        .id  = "/chip/rcc",
        .reg = AHB1PERIPH_BASE + 0x1000UL,
    },

    {
        .id     = "/chip/sdmmc@1",
        .reg    = AHB2PERIPH_BASE + 0x08062400UL,
        .config = sdmmc_phy_config_type
        {
            .clock_edge      = sdmmc_clock_edge_type::k_clock_rising_edge,
            .power_save      = false,
            .bus_width       = sdmmc_bus_width_type::k_bus_width_one_bit,
            .hw_flow_control = sdmmc_hardware_flow_control_type::k_disable,
            .tranceiver      = sdmmc_tranceiver_type::k_disable,
            .pins            =
            {
                "/chip/gpio-c@0,8",
                "/chip/gpio-c@0,9",
                "/chip/gpio-c@0,10",
                "/chip/gpio-c@0,11",
                "/chip/gpio-c@0,12",
                "/chip/gpio-d@0,2"
            }
        }
    },

    {
        .id     = "/chip/i2c@1",
        .reg    = APB1PERIPH_BASE + 0x5400UL,
        .config = i2c_phy_config_type
        {
            .scl_pin = "/chip/gpio-b@0,8",
            .sda_pin = "/chip/gpio-b@0,9",
        }
    },

    {
        .id     = "/chip/i2c@2",
        .reg    = APB1PERIPH_BASE + 0x5800UL,
        .config = i2c_phy_config_type
        {
            .scl_pin = "/chip/gpio-b@0,10",
            .sda_pin = "/chip/gpio-b@0,11",
        }
    },

    {
        .id     = "/chip/i2c@3",
        .reg    = APB1PERIPH_BASE + 0x5C00UL,
        .config = i2c_phy_config_type
        {
            .scl_pin = "/chip/gpio-c@0,0",
            .sda_pin = "/chip/gpio-c@0,1",
        }
    },

    {
        .id     = "/chip/usb-otg",
        .reg    = USB_OTG_FS_PERIPH_BASE,
        .config = usb_phy_config_type
        {
            .dp_pin = "/chip/gpio-a@0,11",
            .dm_pin = "/chip/gpio-a@0,12",
        }
    },

    {
        .id     = "/chip/octospi@1",
        .reg    = OCTOSPI1_R_BASE,
        .config = ospi_phy_config_type
        {
            .pins =
            {
                "/chip/gpio-a@0,2",
                "/chip/gpio-a@0,3",
                "/chip/gpio-a@0,6",
                "/chip/gpio-a@0,7",
                "/chip/gpio-b@0,0",
                "/chip/gpio-b@0,1"
            }
        }
    },

    {
        .id  = "/chip/gpio-a@0",
        .reg = AHB2PERIPH_BASE + 0x0000UL,
    },

    {
        .id  = "/chip/gpio-b@0",
        .reg = AHB2PERIPH_BASE + 0x0400UL,
    },

    {
        .id  = "/chip/gpio-c@0",
        .reg = AHB2PERIPH_BASE + 0x0800UL,
    },

    {
        .id  = "/chip/gpio-d@0",
        .reg = AHB2PERIPH_BASE + 0x0C00UL,
    },

    {
        .id  = "/chip/gpio-e@0",
        .reg = AHB2PERIPH_BASE + 0x1000UL,
    },

    {
        .id  = "/chip/gpio-f@0",
        .reg = AHB2PERIPH_BASE + 0x1400UL,
    },

    {
        .id  = "/chip/gpio-g@0",
        .reg = AHB2PERIPH_BASE + 0x1800UL,
    },

    {
        .id  = "/chip/gpio-h@0",
        .reg = AHB2PERIPH_BASE + 0x1C00UL,
    },
};

consteval auto peripheral_of(const device_node node) -> auto&
{
    const auto is_node = [=](const auto& device) { return device.id == node; };

    const auto result = std::find_if(std::cbegin(k_peripheral_tree), std::cend(k_peripheral_tree), is_node);
    if ( result != std::cend(k_peripheral_tree) )
    {
        return *result;
    }
}

} // namespace sys::hal

namespace sys {

template <device_node k_node>
struct register_address_of_node
{
    static constexpr auto value = []()
    {
        return hal::peripheral_of(k_node).reg;
    }();
};

} // namespace sys

