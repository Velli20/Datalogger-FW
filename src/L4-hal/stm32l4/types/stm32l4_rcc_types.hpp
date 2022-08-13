#pragma once

namespace sys::hal {

enum class rcc_system_clock_source_type : std::uint32_t
{
    k_multi_speed_internal  = 0b00,
    k_high_speed_internal16 = 0b01,
    k_high_speed_external   = 0b10,
    k_phased_locked_loop    = 0b11,
};

enum class rcc_main_pll_entry_clock_source_type : std::uint32_t
{
    k_no_clock                = 0b00,
    k_multi_speed_internal    = 0b01,
    k_high_speed_internal16   = 0b10,
    k_high_speed_external     = 0b11,
};

enum class rcc_main_pll_input_clock_division_factor_type : std::uint32_t
{
    k_factor1 = 0b0000,
    k_factor2 = 0b0001,
    k_factor3 = 0b0010,
    k_factor4 = 0b0011,
    k_factor5 = 0b0100,
    k_factor6 = 0b0101,
    k_factor7 = 0b0110,
    k_factor8 = 0b0111
};

enum class rcc_main_pll_system_clock_divison_factor_type : std::uint32_t
{
    k_factor2 = 0b00,
    k_factor4 = 0b01,
    k_factor6 = 0b10,
    k_factor8 = 0b11
};

enum class rcc_main_pll48_mhz_clock_division_factor_type : std::uint32_t
{
    k_factor2 = 0b00,
    k_factor4 = 0b01,
    k_factor6 = 0b10,
    k_factor8 = 0b11,
};

enum class rcc_msi_clock_range_type : std::uint32_t
{
    k_range0   = 0b0000,  // Around 100 kHz
    k_range1   = 0b0001,  // Around 200 kHz
    k_range2   = 0b0010,  // Around 400 kHz
    k_range3   = 0b0011,  // Around 800 kHz
    k_range4   = 0b0100,  // Around 1M Hz
    k_range5   = 0b0101,  // Around 2 MHz
    k_range6   = 0b0110,  // Around 4 MHz (reset value)
    k_range7   = 0b0111,  // Around 8 MHz
    k_range8   = 0b1000,  // Around 16 MHz
    k_range9   = 0b1001,  // Around 24 MHz
    k_range10  = 0b1010,  // Around 32 MHz
    k_range11  = 0b1011,  // Around 48 MHz
};

enum class rcc_msi_clock_range_provider_type : std::uint32_t
{
    k_control_and_status_register = 0b0,
    k_clock_control_register      = 0b0,
};

enum class rcc_clock_selection_type : std::uint32_t
{
    k_none = 0,
    k_pll_sai_r,
    k_pll_sai_q,
    k_pll_q,
    k_pclk,
    k_system_clock,
    k_reserved_option,
    k_high_speed_internal48,
    k_high_speed_internal16,
    k_multi_speed_internal,
    k_low_speed_internal,
    k_low_speed_external,
    k_pll48_m1,
    k_48_mhz,
    k_pll_sai3, // (PLLP)
};

enum class rcc_register_type : std::uint32_t
{
    k_ahb1_peripheral_reset = 0, /*!< RCC AHB1 peripheral reset register,                                        Address offset: 0x28 */
    k_ahb2_peripheral_reset,     /*!< RCC AHB2 peripheral reset register,                                        Address offset: 0x2C */
    k_ahb3_peripheral_reset,     /*!< RCC AHB3 peripheral reset register,                                        Address offset: 0x30 */
    k_reserved1,                 /*!< Reserved,                                                                  Address offset: 0x34 */
    k_apb1_peripheral_reset1,    /*!< RCC APB1 peripheral reset register 1,                                      Address offset: 0x38 */
    k_apb1_peripheral_reset2,    /*!< RCC APB1 peripheral reset register 2,                                      Address offset: 0x3C */
    k_apb2_peripheral_reset,     /*!< RCC APB2 peripheral reset register,                                        Address offset: 0x40 */
    k_reserved2,                 /*!< Reserved,                                                                  Address offset: 0x44 */
    k_ahb1_clocks_enable,        /*!< RCC AHB1 peripheral clocks enable register,                                Address offset: 0x48 */
    k_ahb2_clocks_enable,        /*!< RCC AHB2 peripheral clocks enable register,                                Address offset: 0x4C */
    k_ahb3_clocks_enable,        /*!< RCC AHB3 peripheral clocks enable register,                                Address offset: 0x50 */
    k_reserved3,                 /*!< Reserved,                                                                  Address offset: 0x54 */
    k_apb1_clocks_enable1,       /*!< RCC APB1 peripheral clocks enable register 1,                              Address offset: 0x58 */
    k_apb1_clocks_enable2,       /*!< RCC APB1 peripheral clocks enable register 2,                              Address offset: 0x5C */
    k_apb2_clocks_enable,        /*!< RCC APB2 peripheral clocks enable register,                                Address offset: 0x60 */
    k_reserved4,                 /*!< Reserved,                                                                  Address offset: 0x64 */
    k_ahb1_smenr,                /*!< RCC AHB1 peripheral clocks enable in sleep and stop modes register,        Address offset: 0x68 */
    k_ahb2_smenr,                /*!< RCC AHB2 peripheral clocks enable in sleep and stop modes register,        Address offset: 0x6C */
    k_ahb3_smenr,                /*!< RCC AHB3 peripheral clocks enable in sleep and stop modes register,        Address offset: 0x70 */
    k_reserved5,                 /*!< Reserved,                                                                  Address offset: 0x74 */
    k_apb1_smenr1,               /*!< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 1, Address offset: 0x78 */
    k_apb1_smenr2,               /*!< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 2, Address offset: 0x7C */
    k_apb2_smenr,                /*!< RCC APB2 peripheral clocks enable in sleep mode and stop modes register,   Address offset: 0x80 */
    k_reserved6,                 /*!< Reserved,                                                                  Address offset: 0x84 */
    k_ccipr,                     /*!< RCC peripherals independent clock configuration register,                  Address offset: 0x88 */
    k_reserved7,                 /*!< Reserved,                                                                  Address offset: 0x8C */
    k_backup_domain,             /*!< RCC backup domain control register,                                        Address offset: 0x90 */
    k_csr,                       /*!< RCC clock control & status register,                                       Address offset: 0x94 */
    k_crrcr,                     /*!< RCC clock recovery RC register,                                            Address offset: 0x98 */
    k_ccipr2,                    /*!< RCC peripherals independent clock configuration register 2,                Address offset: 0x9C */
    k_none,
    k_count = k_ccipr2 + 1
};

} // namespace sys::hal

namespace sys {

template <>
struct is_bitmask_enum <hal::rcc_system_clock_source_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::rcc_main_pll_entry_clock_source_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::rcc_main_pll_system_clock_divison_factor_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::rcc_msi_clock_range_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::rcc_msi_clock_range_provider_type> : std::true_type {};

} // namespace sys
