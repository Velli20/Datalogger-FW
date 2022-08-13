#pragma once

namespace sys::hal {

consteval auto gpio_pin_of(const device_node node)
{
    constexpr struct
    {
        device_node          node;
        device_node          compatible;
        gpio_phy_config_type config;
    } k_pin_tree[] =
    {
        {
            .node       = "/chip/gpio-b@0,14",
            .compatible = "24-DC-Channel-1",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_input,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-b@0,13",
            .compatible = "24-DC-Channel-2",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_input,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-b@0,15",
            .compatible = "24-DC-Channel-3",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_input,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-d@0,8",
            .compatible = "24-DC-Channel-4",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_input,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-d@0,10",
            .compatible = "24-DC-Channel-5",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_input,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-d@0,9",
            .compatible = "24-DC-Channel-6",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_input,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-d@0,11",
            .compatible = "24-DC-Channel-7",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_input,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-d@0,12",
            .compatible = "24-DC-Channel-8",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_input,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-d@0,14",
            .compatible = "Joystick-Center",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_interrupt_falling,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-d@0,13",
            .compatible = "Joystick-Left",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_interrupt_falling,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-d@0,15",
            .compatible = "Joystick-Down",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_interrupt_falling,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-c@0,7",
            .compatible = "Joystick-Up",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_interrupt_falling,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-c@0,6",
            .compatible = "Joystick-Right",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_interrupt_falling,
                .pull      = gpio_pull_type::k_pull_down,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_medium,
            },
        },

        {
            .node       = "/chip/gpio-a@0,9",
            .compatible = "/chip/gpio/usart1-tx",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_usart1,
                .speed     = gpio_speed_type::k_speed_high,
            },
        },

        {
            .node       = "/chip/gpio-a@0,10",
            .compatible = "/chip/gpio/usart1-rx",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_usart1,
                .speed     = gpio_speed_type::k_speed_high,
            }
        },

        {
            .node       = "/chip/gpio-a@0,0",
            .compatible = "/chip/gpio/uart4-tx",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_uart4,
                .speed     = gpio_speed_type::k_speed_medium,
            }
        },

        {
            .node       = "/chip/gpio-a@0,1",
            .compatible = "/chip/gpio/uart4-rx",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_uart4,
                .speed     = gpio_speed_type::k_speed_medium,
            }
        },

        {
            .node       = "/chip/gpio-e@0,6",
            .compatible = "LED1-Pin",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_output_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_low,
            }
        },

        {
            .node       = "/chip/gpio-c@0,13",
            .compatible = "LED2-Pin",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_output_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_low,
            }
        },

        {
            .node       = "/chip/gpio-c@0,8",
            .compatible = "SDMMC1-D0",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_sdmmc1,
                .speed     = gpio_speed_type::k_speed_high,
            }
        },

        {
            .node       = "/chip/gpio-c@0,9",
            .compatible = "SDMMC1-D1",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_sdmmc1,
                .speed     = gpio_speed_type::k_speed_high,
            }
        },

        {
            .node       = "/chip/gpio-c@0,10",
            .compatible = "SDMMC1-D2",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_sdmmc1,
                .speed     = gpio_speed_type::k_speed_high,
            }
        },

        {
            .node       = "/chip/gpio-c@0,11",
            .compatible = "SDMMC1-D3",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_sdmmc1,
                .speed     = gpio_speed_type::k_speed_high,
            }
        },

        {
            .node       = "/chip/gpio-c@0,12",
            .compatible = "SDMMC1-CK",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_sdmmc1,
                .speed     = gpio_speed_type::k_speed_high,
            }
        },

        {
            .node       = "/chip/gpio-d@0,2",
            .compatible = "SDMMC1-CMD",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_sdmmc1,
                .speed     = gpio_speed_type::k_speed_high,
            }
        },

        {
            .node       = "/chip/gpio-b@0,9",
            .compatible = "I2C1-SDA",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_i2c1,
                .speed     = gpio_speed_type::k_speed_medium,
            }
        },

        {
            .node       = "/chip/gpio-b@0,8",
            .compatible = "I2C1-SCL",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_i2c1,
                .speed     = gpio_speed_type::k_speed_medium,
            }
        },

        {
            .node       = "/chip/gpio-b@0,11",
            .compatible = "I2C2-SDA",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_i2c2,
                .speed     = gpio_speed_type::k_speed_medium,
            }
        },

        {
            .node       = "/chip/gpio-b@0,10",
            .compatible = "I2C2-SCL",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_i2c2,
                .speed     = gpio_speed_type::k_speed_medium,
            }
        },

        {
            .node       = "/chip/gpio-c@0,1",
            .compatible = "I2C3-SDA",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_i2c3,
                .speed     = gpio_speed_type::k_speed_medium,
            }
        },

        {
            .node       = "/chip/gpio-c@0,0",
            .compatible = "I2C3-SCL",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_i2c3,
                .speed     = gpio_speed_type::k_speed_medium,
            }
        },

        {
            .node       = "/chip/gpio-a@0,11",
            .compatible = "USB-OTG-DM",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_usb_otg,
                .speed     = gpio_speed_type::k_speed_highest,
            }
        },

        {
            .node       = "/chip/gpio-a@0,12",
            .compatible = "USB-OTG-DP",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_usb_otg,
                .speed     = gpio_speed_type::k_speed_highest,
            }
        },

        {
            .node       = "/chip/gpio-b@0,2",
            .compatible = "/gpio/WiFi-Reset", /* RST */
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_output_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_low,
            }
        },

        {
            .node       = "/chip/gpio-e@0,7",
            .compatible = "/gpio/WiFi-Wakeup", /* GPIO16 (used for Deep-sleep wake-up when connected to RST pin.) */
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_output_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_low,
            }
        },

        {
            .node       = "/chip/gpio-e@0,11",
            .compatible = "/gpio/WiFi-Download", /* GPIO0 (UART download: pull down. Flash boot: floating or pull up.) */
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_output_push_pull,
                .pull      = gpio_pull_type::k_pull_up,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_high,
            }
        },

        {
            .node       = "/chip/gpio-e@0,12",
            .compatible = "/gpio/WiFi-SPI-CS", /* GPIO15 (MTDO; HSPICS; UART0_RTS Pull down.) */
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_output_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_low,
            }
        },

        {
            .node       = "/chip/gpio-e@0,14",
            .compatible = "/gpio/WiFi-UART_CTS", /* GPIO13 (GPIO13; HSPI_MOSI; UART0_CTS) */
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_output_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_low,
            }
        },

        {
            .node       = "/chip/gpio-e@0,15",
            .compatible = "/gpio/WiFi-UART_CTS", /* GPIO12 (HSPI_MISO) */
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_output_open_drain,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_none,
                .speed     = gpio_speed_type::k_speed_low,
            }
        },

        {
            .node       = "/chip/gpio-a@0,2",
            .compatible = "OCTOSPI1-NCS",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_octospi1,
                .speed     = gpio_speed_type::k_speed_highest,
            }
        },

        {
            .node       = "/chip/gpio-a@0,3",
            .compatible = "OCTOSPI1-CLK",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_octospi1,
                .speed     = gpio_speed_type::k_speed_highest,
            }
        },

        {
            .node       = "/chip/gpio-a@0,6",
            .compatible = "OCTOSPI1-IO3",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_octospi1,
                .speed     = gpio_speed_type::k_speed_highest,
            }
        },

        {
            .node       = "/chip/gpio-a@0,7",
            .compatible = "OCTOSPI1-IO2",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_octospi1,
                .speed     = gpio_speed_type::k_speed_highest,
            }
        },

        {
            .node       = "/chip/gpio-b@0,0",
            .compatible = "OCTOSPI1-IO1",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_octospi1,
                .speed     = gpio_speed_type::k_speed_highest,
            }
        },

        {
            .node       = "/chip/gpio-b@0,1",
            .compatible = "OCTOSPI1-IO0",
            .config     = gpio_phy_config_type
            {
                .mode      = gpio_mode_type::k_alternate_push_pull,
                .pull      = gpio_pull_type::k_none,
                .alternate = gpio_alternate_type::k_octospi1,
                .speed     = gpio_speed_type::k_speed_highest,
            }
        },
    };

    const auto predicate = [=](const auto& pin)
    {
        return (pin.compatible == node) || (pin.node == node && pin.node.m_unit_sub_address == node.m_unit_sub_address);
    };

    const auto result = std::find_if(std::cbegin(k_pin_tree), std::cend(k_pin_tree), predicate);
    if ( result != std::cend(k_pin_tree) )
    {
        return *result;
    }
}

template <std::size_t N>
consteval auto gpio_pin_of(const char(&name)[N])
{
    return hal::gpio_pin_of(device_node{name});
}

} // namespace sys::hal
