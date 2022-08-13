#pragma once

namespace sys::hal {

struct msi_range_table
{
    [[nodiscard]]  static constexpr auto get(const rcc_msi_clock_range_type range) -> std::uint32_t
    {
        using namespace sys::units::literals;

        if ( range == rcc_msi_clock_range_type::k_range0  ) { return 100_kHz; }
        if ( range == rcc_msi_clock_range_type::k_range1  ) { return 200_kHz; }
        if ( range == rcc_msi_clock_range_type::k_range2  ) { return 400_kHz; }
        if ( range == rcc_msi_clock_range_type::k_range3  ) { return 800_kHz; }
        if ( range == rcc_msi_clock_range_type::k_range4  ) { return 1_MHz;   }
        if ( range == rcc_msi_clock_range_type::k_range5  ) { return 2_MHz;   }
        if ( range == rcc_msi_clock_range_type::k_range6  ) { return 4_MHz;   }
        if ( range == rcc_msi_clock_range_type::k_range7  ) { return 8_MHz;   }
        if ( range == rcc_msi_clock_range_type::k_range8  ) { return 16_MHz;  }
        if ( range == rcc_msi_clock_range_type::k_range9  ) { return 24_MHz;  }
        if ( range == rcc_msi_clock_range_type::k_range10 ) { return 32_MHz;  }
        if ( range == rcc_msi_clock_range_type::k_range11 ) { return 48_MHz;  }

        debug_assert(false);

        return {};
    }
};

namespace rcc::detail {

template <device_node k_node>
struct traits
{
    private:

    static constexpr struct
    {
        device_node          node;
        rcc_register_type    enable_register;
        std::uint32_t        enable_bit_position;
        rcc_register_type    reset_register;
        std::uint32_t        reset_bit_position;
    } k_rcc_table[] =
    {
        { "/chip/dma@1",                rcc_register_type::k_ahb1_clocks_enable,   RCC_AHB1ENR_DMA1EN_Pos,    rcc_register_type::k_ahb1_peripheral_reset,  RCC_AHB1RSTR_DMA1RST_Pos    },
        { "/chip/dma@2",                rcc_register_type::k_ahb1_clocks_enable,   RCC_AHB1ENR_DMA2EN_Pos,    rcc_register_type::k_ahb1_peripheral_reset,  RCC_AHB1RSTR_DMA2RST_Pos    },
        { "/chip/dma-mux",              rcc_register_type::k_ahb1_clocks_enable,   RCC_AHB1ENR_DMAMUX1EN_Pos, rcc_register_type::k_ahb1_peripheral_reset,  RCC_AHB1RSTR_DMAMUX1RST_Pos },
        { "/chip/dma2d",                rcc_register_type::k_ahb1_clocks_enable,   RCC_AHB1ENR_DMA2DEN_Pos,   rcc_register_type::k_ahb1_peripheral_reset,  RCC_AHB1RSTR_DMA2DRST_Pos   },
        { "/chip/flash",                rcc_register_type::k_ahb1_clocks_enable,   RCC_AHB1ENR_FLASHEN_Pos,   rcc_register_type::k_ahb1_peripheral_reset,  RCC_AHB1RSTR_FLASHRST_Pos   },
        { "/chip/gpio-a@0",             rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_GPIOAEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_GPIOARST_Pos   },
        { "/chip/gpio-b@0",             rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_GPIOBEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_GPIOBRST_Pos   },
        { "/chip/gpio-c@0",             rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_GPIOCEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_GPIOCRST_Pos   },
        { "/chip/gpio-d@0",             rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_GPIODEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_GPIODRST_Pos   },
        { "/chip/gpio-e@0",             rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_GPIOEEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_GPIOERST_Pos   },
        { "/chip/gpio-f@0",             rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_GPIOFEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_GPIOFRST_Pos   },
        { "/chip/gpio-g@0",             rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_GPIOGEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_GPIOGRST_Pos   },
        { "/chip/gpio-h@0",             rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_GPIOHEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_GPIOHRST_Pos   },
        { "/chip/sdmmc@1",              rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_SDMMC1EN_Pos,  rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_SDMMC1RST_Pos  },
        { "/chip/rng",                  rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_RNGEN_Pos,     rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_RNGRST_Pos     },
        { "/chip/usb-otg",              rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_OTGFSEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_OTGFSRST_Pos   },
        { "/chip/octospi-m",            rcc_register_type::k_ahb2_clocks_enable,   RCC_AHB2ENR_OSPIMEN_Pos,   rcc_register_type::k_ahb2_peripheral_reset,  RCC_AHB2RSTR_OSPIMRST_Pos   },
        { "/chip/octospi@1",            rcc_register_type::k_ahb3_clocks_enable,   RCC_AHB3ENR_OSPI1EN_Pos,   rcc_register_type::k_ahb3_peripheral_reset,  RCC_AHB3RSTR_OSPI1RST_Pos   },
        { "/chip/octospi@2",            rcc_register_type::k_ahb3_clocks_enable,   RCC_AHB3ENR_OSPI2EN_Pos,   rcc_register_type::k_ahb3_peripheral_reset,  RCC_AHB3RSTR_OSPI2RST_Pos   },
        { "/chip/fmc",                  rcc_register_type::k_ahb3_clocks_enable,   RCC_AHB3ENR_FMCEN_Pos,     rcc_register_type::k_ahb3_peripheral_reset,  RCC_AHB3RSTR_FMCRST_Pos     },
        { "/chip/i2c@1",                rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_I2C1EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_I2C1RST_Pos   },
        { "/chip/i2c@2",                rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_I2C2EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_I2C2RST_Pos   },
        { "/chip/i2c@3",                rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_I2C3EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_I2C3RST_Pos   },
        { "/chip/usart@2",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_USART2EN_Pos, rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_USART2RST_Pos },
        { "/chip/usart@3",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_USART3EN_Pos, rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_USART3RST_Pos },
        { "/chip/uart@4",               rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_UART4EN_Pos,  rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_UART4RST_Pos  },
        { "/chip/uart@5",               rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_UART5EN_Pos,  rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_UART5RST_Pos  },
        { "/chip/lptim@1",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_LPTIM1EN_Pos, rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_LPTIM1RST_Pos },
        { "/chip/spi@2",                rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_SPI2EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_SPI2RST_Pos   },
        { "/chip/spi@3",                rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_SPI3EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_SPI3RST_Pos   },
        { "/chip/power",                rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_PWREN_Pos,    rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_PWRRST_Pos    },
        { "/chip/timer@2",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_TIM2EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_TIM2RST_Pos   },
        { "/chip/timer@3",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_TIM3EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_TIM3RST_Pos   },
        { "/chip/timer@4",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_TIM4EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_TIM4RST_Pos   },
        { "/chip/timer@5",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_TIM5EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_TIM5RST_Pos   },
        { "/chip/timer@6",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_TIM6EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_TIM6RST_Pos   },
        { "/chip/timer@7",              rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_TIM7EN_Pos,   rcc_register_type::k_apb1_peripheral_reset1, RCC_APB1RSTR1_TIM7RST_Pos   },
        { "/chip/watchdoq",             rcc_register_type::k_apb1_clocks_enable1,  RCC_APB1ENR1_WWDGEN_Pos,   rcc_register_type::k_none,                   0                           },
        { "/chip/i2c@4",                rcc_register_type::k_apb1_clocks_enable2,  RCC_APB1ENR2_I2C4EN_Pos,   rcc_register_type::k_apb1_peripheral_reset2, RCC_APB1RSTR2_I2C4RST_Pos   },
        { "/chip/lptim@2",              rcc_register_type::k_apb1_clocks_enable2,  RCC_APB1ENR2_LPTIM2EN_Pos, rcc_register_type::k_apb1_peripheral_reset2, RCC_APB1RSTR2_LPTIM2RST_Pos },
        { "/chip/sai@1",                rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_SAI1EN_Pos,    rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_SAI1RST_Pos    },
        { "/chip/sai@2",                rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_SAI2EN_Pos,    rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_SAI2RST_Pos    },
        { "/chip/timer@1",              rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_TIM1EN_Pos,    rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_TIM1RST_Pos    },
        { "/chip/timer@8",              rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_TIM8EN_Pos,    rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_TIM8RST_Pos    },
        { "/chip/timer@15",             rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_TIM15EN_Pos,   rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_TIM15RST_Pos   },
        { "/chip/timer@16",             rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_TIM16EN_Pos,   rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_TIM16RST_Pos   },
        { "/chip/timer@17",             rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_TIM17EN_Pos,   rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_TIM17RST_Pos   },
        { "/chip/spi@1",                rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_SPI1EN_Pos,    rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_SPI1RST_Pos    },
        { "/chip/system-configuration", rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_SYSCFGEN_Pos,  rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_SYSCFGRST_Pos  },
        { "/chip/usart@1",              rcc_register_type::k_apb2_clocks_enable,   RCC_APB2ENR_USART1EN_Pos,  rcc_register_type::k_apb2_peripheral_reset,  RCC_APB2RSTR_USART1RST_Pos  },
        { "/chip/rtc",                  rcc_register_type::k_backup_domain,        RCC_BDCR_RTCEN_Pos,        rcc_register_type::k_none,                   0                           },
        { "/chip/cortex-m-systick",     rcc_register_type::k_backup_domain,        RCC_BDCR_RTCEN_Pos,        rcc_register_type::k_none,                   0                           },
    };

    static consteval auto rcc_helper()
    {
        auto res = std::find_if(std::begin(k_rcc_table), std::end(k_rcc_table), [](auto& i) { return i.node == k_node; });
        if ( res != std::end(k_rcc_table) )
        {
            return std::make_tuple(
                res->enable_register,
                res->enable_bit_position,
                res->reset_register,
                res->reset_bit_position
            );
        }
    }

    public:

    static constexpr auto k_enable_bit_position = std::get<1>(rcc_helper());
    static constexpr auto k_enable_register     = std::get<0>(rcc_helper());
    static constexpr auto k_reset_register      = std::get<2>(rcc_helper());
    static constexpr auto k_reset_bit_position  = std::get<3>(rcc_helper());
};

} // namespace rcc::detail

/* RM0432 page 305 */
constexpr std::tuple<rcc_register_type, device_node, rcc_clock_selection_type, rcc_clock_selection_type, rcc_clock_selection_type, rcc_clock_selection_type, std::uint32_t> k_rcc_clock_source[] =
{
    /*       mmio::register_type             peripheral                      Bit 00:                                            Bit 01:                                           Bit 10:                                            Bit 11: */
    { rcc_register_type::k_ccipr,  device_node{"/chip/adc"},       rcc_clock_selection_type::k_none,                  rcc_clock_selection_type::k_pll_sai_r,            rcc_clock_selection_type::k_reserved_option,       rcc_clock_selection_type::k_system_clock,         RCC_CCIPR_ADCSEL_Pos    },
    { rcc_register_type::k_ccipr,  device_node{"/chip/usb-otg"},   rcc_clock_selection_type::k_high_speed_internal48, rcc_clock_selection_type::k_pll_sai_q,            rcc_clock_selection_type::k_pll_q,                 rcc_clock_selection_type::k_multi_speed_internal, RCC_CCIPR_CLK48SEL_Pos  },
    { rcc_register_type::k_ccipr,  device_node{"/chip/rng"},       rcc_clock_selection_type::k_high_speed_internal48, rcc_clock_selection_type::k_pll_sai_q,            rcc_clock_selection_type::k_pll_q,                 rcc_clock_selection_type::k_multi_speed_internal, RCC_CCIPR_CLK48SEL_Pos  },
    { rcc_register_type::k_ccipr,  device_node{"/chip/lptim@1"},   rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_low_speed_internal,   rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_low_speed_external,   RCC_CCIPR_LPTIM2SEL_Pos },
    { rcc_register_type::k_ccipr,  device_node{"/chip/i2c@3"},     rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_reserved_option,      RCC_CCIPR_I2C3SEL_Pos   },
    { rcc_register_type::k_ccipr,  device_node{"/chip/i2c@2"},     rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_reserved_option,      RCC_CCIPR_I2C2SEL_Pos   },
    { rcc_register_type::k_ccipr,  device_node{"/chip/i2c@1"},     rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_reserved_option,      RCC_CCIPR_I2C1SEL_Pos   },
    { rcc_register_type::k_ccipr,  device_node{"/chip/lpuart@1"},  rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_low_speed_external,   RCC_CCIPR_LPUART1SEL    },
    { rcc_register_type::k_ccipr,  device_node{"/chip/uart@5"},    rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_low_speed_external,   RCC_CCIPR_UART5SEL_Pos  },
    { rcc_register_type::k_ccipr,  device_node{"/chip/uart@4"},    rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_low_speed_external,   RCC_CCIPR_UART4SEL_Pos  },
    { rcc_register_type::k_ccipr,  device_node{"/chip/usart@3"},   rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_low_speed_external,   RCC_CCIPR_USART3SEL_Pos },
    { rcc_register_type::k_ccipr,  device_node{"/chip/usart@2"},   rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_low_speed_external,   RCC_CCIPR_USART2SEL_Pos },
    { rcc_register_type::k_ccipr,  device_node{"/chip/usart@1"},   rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_low_speed_external,   RCC_CCIPR_USART1SEL_Pos },
    { rcc_register_type::k_ccipr2, device_node{"/chip/octospi@1"}, rcc_clock_selection_type::k_system_clock,          rcc_clock_selection_type::k_multi_speed_internal, rcc_clock_selection_type::k_pll48_m1,              rcc_clock_selection_type::k_reserved_option,      RCC_CCIPR2_OSPISEL_Pos  },
    { rcc_register_type::k_ccipr2, device_node{"/chip/octospi@2"}, rcc_clock_selection_type::k_system_clock,          rcc_clock_selection_type::k_multi_speed_internal, rcc_clock_selection_type::k_pll48_m1,              rcc_clock_selection_type::k_reserved_option,      RCC_CCIPR2_OSPISEL_Pos  },
    { rcc_register_type::k_ccipr2, device_node{"/chip/octospi-m"}, rcc_clock_selection_type::k_system_clock,          rcc_clock_selection_type::k_multi_speed_internal, rcc_clock_selection_type::k_pll48_m1,              rcc_clock_selection_type::k_reserved_option,      RCC_CCIPR2_OSPISEL_Pos  },
    { rcc_register_type::k_ccipr2, device_node{"/chip/i2c@4"},     rcc_clock_selection_type::k_pclk,                  rcc_clock_selection_type::k_system_clock,         rcc_clock_selection_type::k_high_speed_internal16, rcc_clock_selection_type::k_reserved_option,      RCC_CCIPR2_I2C4SEL_Pos  },
    { rcc_register_type::k_ccipr2, device_node{"/chip/sdmmc@1"},   rcc_clock_selection_type::k_48_mhz,                rcc_clock_selection_type::k_pll_sai3,             rcc_clock_selection_type::k_reserved_option,       rcc_clock_selection_type::k_reserved_option,      RCC_CCIPR2_SDMMCSEL_Pos }
};

template <device_node k_node>
static consteval auto find_clock_register_bits()
{
    for ( const auto& [reg, peripheral, bit00, bit01, bit10, bit11, reg_offset] : k_rcc_clock_source )
    {
        if ( peripheral == k_node )
        {
            return std::make_tuple(reg, reg_offset, bit00, bit01, bit10, bit11);
        }
    }
}

template <>
struct peripheral_type<"/chip/rcc"> final
{
    private:

    [[nodiscard]] static constexpr auto get() noexcept -> RccLayout&
    {
        constexpr std::uintptr_t k_rcc_base = register_address_of_node<"/chip/rcc">::value;

        return mmio::place_at<RccLayout>(k_rcc_base);
    }

    template <rcc_register_type k_register>
    [[nodiscard]] static constexpr auto get() noexcept
    {
        constexpr std::uintptr_t k_rcc_base = register_address_of_node<"/chip/rcc">::value;

        return mmio::place_at<RccLayout>(k_rcc_base).Clocks[static_cast<std::size_t>(k_register)];
    }

    template <rcc_register_type k_register, std::uint32_t k_field_width, std::uint32_t k_field_offset>
    [[nodiscard]] static constexpr auto get() noexcept
    {
        constexpr std::uintptr_t k_rcc_base = register_address_of_node<"/chip/rcc">::value;

        return mmio::place_at<RccLayout>(k_rcc_base).Clocks[static_cast<std::size_t>(k_register)].Field<k_field_width, k_field_offset>();
    }

    public:

    [[maybe_unused]] static constexpr auto init()
    {
        // Reset Clock Configuation register.

        get().ClockConfiguration.Clear();

        // Clear HSEON, CSSON , HSION, and PLLON bits.

        get().ClockControl
             .Edit()
             .Clear<RccLayout::ClockControl::HseClockEnable>()
             .Clear<RccLayout::ClockControl::ClockSecuritySystemEnable>()
             .Clear<RccLayout::ClockControl::HsiClockEnable>()
             .Clear<RccLayout::ClockControl::MainPllEnable>()
             .Save();

        // Clear PLL configuration register.

        get().SystemPllConfiguration.Clear();

        // Clear HSE Crystal Oscillator Bypass bit and enable Multi Speed Internal clock.

        get().ClockControl
             .Edit()
             .Clear<RccLayout::ClockControl::HseCrystalOscillatorBypass>()
             .Set  <RccLayout::ClockControl::MsiClockEnable>()
             .Save();

        // Disable all interrupts.

        get().ClockInterruptEnable.Clear();

        return true;
    }

    template <device_node k_node>
    static constexpr void enable() noexcept
    {
        using traits_type = rcc::detail::traits<k_node>;

        get<traits_type::k_enable_register, 1ul, traits_type::k_enable_bit_position>().Set();

        // The DSB/ISB instructions are recommended by ARM after programming the control registers.

        arch::data_synchronization_barrier();
        arch::instruction_synchronization_barrier();
    }

    template <device_node k_node>
    static constexpr void disable() noexcept
    {
        using traits_type = rcc::detail::traits<k_node>;

        get<traits_type::k_enable_register, 1ul, traits_type::k_enable_bit_position>().Clear();

        // The DSB/ISB instructions are recommended by ARM after programming the control registers.

        arch::data_synchronization_barrier();
        arch::instruction_synchronization_barrier();
    }

    template <device_node k_node>
    [[nodiscard]] static constexpr auto is_enabled() noexcept
    {
        using traits_type = rcc::detail::traits<k_node>;

        return get<traits_type::k_enable_register, 1ul, traits_type::k_enable_bit_position>().Read() != 0;
    }

    template <device_node k_node>
    static constexpr void reset() noexcept
    {
        using traits_type = rcc::detail::traits<k_node>;

        // Set reset bit...

        get<traits_type::k_reset_register, 1ul, traits_type::k_reset_bit_position>().Set();

        arch::data_synchronization_barrier();
        arch::instruction_synchronization_barrier();

        // ... and clear it to release reset.

        get<traits_type::k_reset_register, 1ul, traits_type::k_reset_bit_position>().Clear();

        arch::data_synchronization_barrier();
        arch::instruction_synchronization_barrier();
    }

    [[nodiscard]] static constexpr auto get_system_clock_frequency() noexcept -> std::uint32_t
    {
        const auto system_clock_source = get().ClockConfiguration.    Read<RccLayout::ClockConfiguration::    SystemClockSwitchStatus>();
        const auto pll_oscsource       = get().SystemPllConfiguration.Read<RccLayout::SystemPllConfiguration::MainPllEntryClockSource>();

        std::uint32_t msi_frequency{};
        std::uint32_t system_clock_frequency{};

        // Check if Multispeed Internal Oscillator is used as system clock source ...

        if ( (system_clock_source == rcc_system_clock_source_type::k_multi_speed_internal) ||
             (system_clock_source == rcc_system_clock_source_type::k_phased_locked_loop    && pll_oscsource == rcc_main_pll_entry_clock_source_type::k_multi_speed_internal) )
        {
            // get Multispeed Internal Oscillator range. Check which register holds the range value.

            if ( get().ClockControl.Read<RccLayout::ClockControl::MsiClockRangeSelection>() == rcc_msi_clock_range_provider_type::k_clock_control_register )
            {
                msi_frequency = msi_range_table::get(get<rcc_register_type::k_csr>().Read<RccLayout::ClockControlAndStatus::MsiRangeAfterStandby>());
            }

            // Read MSI value from Clock COntrol register.

            else
            {
                msi_frequency = msi_range_table::get(get().ClockControl.Read<RccLayout::ClockControl::MsiClockRange>());
            }

            /*MSI frequency range in HZ*/

            if ( system_clock_source == rcc_system_clock_source_type::k_multi_speed_internal )
            {
                /* MSI used as system clock source */
                system_clock_frequency = msi_frequency;
            }
        }

        // ... or High Speed Internal ...

        else if ( system_clock_source == rcc_system_clock_source_type::k_high_speed_internal16 )
        {
            system_clock_frequency = HSI_VALUE;
        }

        // ... or High Speed External ...

        else if ( system_clock_source == rcc_system_clock_source_type::k_high_speed_external )
        {
            system_clock_frequency = HSE_VALUE;
        }

        // ... error. No system clock source selected.

        else
        {
            debug_assert(false);
        }

        if ( system_clock_source == rcc_system_clock_source_type::k_phased_locked_loop )
        {
            // From refrence manual RM0432 page 264:
            // Section 6.4.4 PLL configuration register (RCC_PLLCFGR):

            /* This register is used to configure the PLL clock outputs according to the formulas:
                • f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
                • f(PLL_P)     = f(VCO clock) / PLLP
                • f(PLL_Q)     = f(VCO clock) / PLLQ
                • f(PLL_R)     = f(VCO clock) / PLLR
            */

            /* PLL used as system clock  source */

            /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE) * PLLN / PLLM
                SYSCLK = PLL_VCO / PLLR
            */

            const auto pllsource = get().SystemPllConfiguration.Read<RccLayout::SystemPllConfiguration::MainPllEntryClockSource>();

            std::uint32_t pllvco{};
            switch ( pllsource )
            {
                case rcc_main_pll_entry_clock_source_type::k_high_speed_internal16:
                    pllvco = HSI_VALUE;
                    break;

                case rcc_main_pll_entry_clock_source_type::k_high_speed_external:
                    pllvco = HSE_VALUE;
                    break;

                case rcc_main_pll_entry_clock_source_type::k_no_clock:
                    [[fallthrough]];

                case rcc_main_pll_entry_clock_source_type::k_multi_speed_internal:
                    pllvco = msi_frequency;
                    break;
            }

            auto factor_m = get().SystemPllConfiguration.Read<RccLayout::SystemPllConfiguration::MainPllInputClockDivisonFactor>();
            auto factor_n = get().SystemPllConfiguration.Read<RccLayout::SystemPllConfiguration::MainPllMultiplicationFactor>();
            auto factor_r = get().SystemPllConfiguration.Read<RccLayout::SystemPllConfiguration::MainPllSystemClockDivisonFactor>();

            std::uint32_t pllm   = (static_cast<std::uint32_t>(factor_m) + 1u);
            std::uint32_t pllr   = (static_cast<std::uint32_t>(factor_r) + 1u) * 2u;
                          pllvco = (static_cast<std::uint32_t>(factor_n) * pllvco) / pllm;

            debug_assert(pllr > 0);

            return pllvco / pllr;
        }

        return system_clock_frequency;
    }

    template <device_node k_node>
    [[nodiscard]] static constexpr auto get_clock_frequency() noexcept -> std::optional<std::uint32_t>
    {
        constexpr auto k_clock_selection = find_clock_register_bits<k_node>();
        constexpr auto k_register        = std::get<0>(k_clock_selection);
        constexpr auto k_registerOffset  = std::get<1>(k_clock_selection);

        const rcc_clock_selection_type source = [&]
        {
            // Read clock source bits.

            switch ( get<k_register, 2ul, k_registerOffset>().Read() )
            {
                case 0ul:
                    return std::get<2>(k_clock_selection);

                case 1ul:
                    return std::get<3>(k_clock_selection);

                case 2ul:
                    return std::get<4>(k_clock_selection);

                case 3ul:
                    return std::get<5>(k_clock_selection);
            }

            return rcc_clock_selection_type::k_none;
        }();

        debug_assert(source != rcc_clock_selection_type::k_none);
        debug_assert(source != rcc_clock_selection_type::k_reserved_option);

        switch ( source )
        {
            case rcc_clock_selection_type::k_pll_sai3:
                break;

            case rcc_clock_selection_type::k_48_mhz:
                return HSI48_VALUE;

            case rcc_clock_selection_type::k_pll_sai_r:
                break;

            case rcc_clock_selection_type::k_pll_sai_q:
                break;

            case rcc_clock_selection_type::k_pll_q:
                break;

            case rcc_clock_selection_type::k_pclk:
                return HAL_RCC_GetPCLK1Freq();

            case rcc_clock_selection_type::k_system_clock:
                return get_system_clock_frequency();

            case rcc_clock_selection_type::k_high_speed_internal48:
                return HSI48_VALUE;

            case rcc_clock_selection_type::k_high_speed_internal16:
                return HSI_VALUE;

            case rcc_clock_selection_type::k_multi_speed_internal:
                return MSI_VALUE;

            case rcc_clock_selection_type::k_low_speed_internal:
                return LSI_VALUE;

            case rcc_clock_selection_type::k_low_speed_external:
                return LSE_VALUE;

            case rcc_clock_selection_type::k_pll48_m1:
                break;

            case rcc_clock_selection_type::k_none:
                [[fallthrough]];

            case rcc_clock_selection_type::k_reserved_option:
                debug_assert(false);
                break;
        }

        return {};
    }
};

} // namespace sys::hal
