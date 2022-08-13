#pragma once

namespace sys::hal {

struct RccLayout
{
    /*!< RCC clock control register,                                                Address offset: 0x00 */

    struct ClockControl final : mmio::register_type<mmio::field_size_type::u32>
    {
        /* MSI clock enable:
            This bit is set and cleared by software.
            Cleared by hardware to stop the MSI oscillator when entering Stop, Standby or Shutdown mode.
            Set by hardware to force the MSI oscillator ON when exiting Standby or Shutdown mode. Set by hardware to force the MSI oscillator ON when STOPWUCK=0 when
            exiting from Stop modes, or in case of a failure of the HSE oscillator
            Set by hardware when used directly or indirectly as system clock.

            Bit 0: MSI oscillator OFF
            Bit 1: MSI oscillator ON
        */

        using MsiClockEnable = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_MSION_Pos>;

        /* MSI clock ready flag:
            This bit is set by hardware to indicate that the MSI oscillator is stable.

            Bit 0: MSI oscillator not ready
            Bit 1: MSI oscillator ready
        */

        using MsiClockReady                            = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_MSIRDY_Pos>;

        /* MSI clock PLL enable:
            Set and cleared by software to enable/ disable the PLL part of the MSI clock source.
            MSIPLLEN must be enabled after LSE is enabled (LSEON enabled) and ready (LSERDY set by hardware).There is a hardware protection to avoid enabling MSIPLLEN if LSE is not ready.
            This bit is cleared by hardware when LSE is disabled (LSEON = 0) or when the Clock Security System on LSE detects a LSE failure (refer to RCC_CSR register).

            Bit 0: MSI PLL OFF
            Bit 1: MSI PLL ON
        */

        using MsiClockPllEnable                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_MSIPLLEN_Pos>;

        /* MSI clock range selection:
            Set by software to select the MSI clock range with MSIRANGE[3:0]. Write 0 has no effect.
            After a standby or a reset MSIRGSEL is at 0 and the MSI range value is provided by MSISRANGE in CSR register.

            Bit 0: MSI Range is provided by MSISRANGE[3:0] in RCC_CSR register.
            Bit 1: MSI Range is provided by MSIRANGE[3:0] in the RCC_CR register.
        */

        using MsiClockRangeSelection                   = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_MSIRGSEL_Pos, rcc_msi_clock_range_provider_type>;

        /* MSI clock range:
            These bits are configured by software to choose the frequency range of MSI when MSIRGSEL is set.12 frequency ranges are available:

            Bit 0000: range 0 around 100 kHz
            Bit 0001: range 1 around 200 kHz
            Bit 0010: range 2 around 400 kHz
            Bit 0011: range 3 around 800 kHz
            Bit 0100: range 4 around 1M Hz
            Bit 0101: range 5 around 2 MHz
            Bit 0110: range 6 around 4 MHz (reset value)
            Bit 0111: range 7 around 8 MHz
            Bit 1000: range 8 around 16 MHz
            Bit 1001: range 9 around 24 MHz
            Bit 1010: range 10 around 32 MHz
            Bit 1011: range 11 around 48 MHz

            Note: Warning: MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is ready (MSIRDY=1).
            MSIRANGE must NOT be modified when MSI is ON and NOT ready (MSION=1 and MSIRDY=0)
        */

        using MsiClockRange                            = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, RCC_CR_MSIRANGE_Pos, rcc_msi_clock_range_type>;

        /* HSI16 clock enable:
            Set and cleared by software.
            Cleared by hardware to stop the HSI16 oscillator when entering Stop, Standby or Shutdown mode.
            Set by hardware to force the HSI16 oscillator ON when STOPWUCK=1 or HSIASFS = 1 when leaving Stop modes, or in case of failure of the HSE crystal oscillator.
            This bit is set by hardware if the HSI16 is used directly or indirectly as system clock.

            Bit 0: HSI16 oscillator OFF
            Bit 1: HSI16 oscillator ON
        */


        using HsiClockEnable                           = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_HSION_Pos>;

        /* HSI16 always enable for peripheral kernels:
            Set and cleared by software to force HSI16 ON even in Stop modes. The HSI16 can only feed USARTs and I2Cs peripherals configured with HSI16 as kernel clock.
            Keeping the HSI16 ON in Stop mode allows to avoid slowing down the communication speed because of the HSI16 startup time. This bit has no effect on HSION value.

            Bit 0: No effect on HSI16 oscillator.
            Bit 1: HSI16 oscillator is forced ON even in Stop mode.
        */

        using HsiClockAlwaysEnabled                    = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_HSIKERON_Pos>;

        /* HSI16 clock ready flag
            Set by hardware to indicate that HSI16 oscillator is stable. This bit is set only when HSI16 is enabled by software by setting HSION.

            Bit 0: HSI16 oscillator not ready
            Bit 1: HSI16 oscillator ready

            Note: Once the HSION bit is cleared, HSIRDY goes low after 6 HSI16 clock cycles.
        */

        using HsiClockReady                            = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_HSIRDY_Pos>;

        /* HSI16 automatic start from Stop
            Set and cleared by software. When the system wakeup clock is MSI, this bit is used to wakeup the HSI16 is parallel of the system wakeup.

            Bit 0: HSI16 oscillator is not enabled by hardware when exiting Stop mode with MSI as wakeup clock.
            Bit 1: HSI16 oscillator is enabled by hardware when exiting Stop mode with MSI as wakeup clock.
        */

        using HsiClockAutomaticStartFromStop           = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_HSIASFS_Pos>;

        /* HSE clock enable
            Set and cleared by software.
            Cleared by hardware to stop the HSE oscillator when entering Stop, Standby or Shutdown mode. This bit cannot be reset if the HSE oscillator is used directly or indirectly as the system clock.

            Bit 0: HSE oscillator OFF
            Bit 1: HSE oscillator ON
        */

        using HseClockEnable                           = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_HSEON_Pos>;

        /* HSE clock ready flag
            Set by hardware to indicate that the HSE oscillator is stable.

            Bit 0: HSE oscillator not ready
            Bit 1: HSE oscillator ready

            Note: Once the HSEON bit is cleared, HSERDY goes low after 6 HSE clock cycles.
        */

        using HseClockReady                            = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_HSERDY_Pos>;

        /* HSE crystal oscillator bypass
            Set and cleared by software to bypass the oscillator with an external clock. The external clock
            must be enabled with the HSEON bit set, to be used by the device. The HSEBYP bit can be written only if the HSE oscillator is disabled.

            Bit 0: HSE crystal oscillator not bypassed
            Bit 1: HSE crystal oscillator bypassed with external clock
        */

        using HseCrystalOscillatorBypass               = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_HSEBYP_Pos>;

        /* Clock security system enable
            Set by software to enable the clock security system. When CSSON is set, the clock detector
            is enabled by hardware when the HSE oscillator is ready, and disabled by hardware if a HSE clock failure is detected. This bit is set only and is cleared by reset.

            Bit 0: Clock security system OFF (clock detector OFF).
            Bit 1: Clock security system ON (Clock detector ON if the HSE oscillator is stable, OFF if not).
        */

        using ClockSecuritySystemEnable                = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_CSSON_Pos>;

        /* Main PLL enable
            Set and cleared by software to enable the main PLL.
            Cleared by hardware when entering Stop, Standby or Shutdown mode. This bit cannot be reset if the PLL clock is used as the system clock.
        */

        using MainPllEnable                            = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_PLLON_Pos>;

        /* Main PLL clock ready flag
            Set by hardware to indicate that the main PLL is locked.

            Bit 0: PLL unlocked
            Bit 1: PLL locked
        */

        using MainPllClockReady                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_PLLRDY_Pos>;

        /* SAI1 PLL enable
            Set and cleared by software to enable PLLSAI1.
            Cleared by hardware when entering Stop, Standby or Shutdown mode.

            Bit 0: PLLSAI1 OFF
            Bit 1: PLLSAI1 ON
        */

        using Sai1PllEnable                            = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_PLLSAI1ON_Pos>;

        /* SAI1 PLL clock ready flag

            Set by hardware to indicate that the PLLSAI1 is locked.

            Bit 0: PLLSAI1 unlocked
            Bit 1: PLLSAI1 locked
        */

        using Sai1PllClockReady                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_PLLSAI1RDY_Pos>;

        /* SAI2 PLL enable
            Set and cleared by software to enable PLLSAI2.
            Cleared by hardware when entering Stop, Standby or Shutdown mode.

            Bit 0: PLLSAI2 OFF
            Bit 1: PLLSAI2 ON
        */

        using Sai2PllEnable                            = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_PLLSAI2ON_Pos>;

        /* SAI2 PLL clock ready flag

            Set by hardware to indicate that the PLLSAI2 is locked.

            Bit 0: PLLSAI2 unlocked
            Bit 1: PLLSAI2 locked
        */

        using Sai2PllClockReady                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CR_PLLSAI2RDY_Pos>;

    } ClockControl;

    mmio::register_type<mmio::field_size_type::u32> InternalClockSourcesCalibration;       /*!< RCC internal clock sources calibration register,                           Address offset: 0x04 */

    /*!< RCC clock configuration register,                                          Address offset: 0x08 */

    struct ClockConfiguration final : mmio::register_type<mmio::field_size_type::u32>
    {
        /* Microcontroller clock output prescaler

            These bits are set and cleared by software.
            It is highly recommended to change this prescaler before MCO output is enabled. 000: MCO is divided by 1

            Bit 001: MCO is divided by 2
            Bit 010: MCO is divided by 4
            Bit 011: MCO is divided by 8 100: MCO is divided by 16
        */

        using ClockOutputPrescaler = mmio::register_mask_type<mmio::field_size_type::u32,  3ul, RCC_CFGR_MCOPRE_Pos>;

        /* Microcontroller clock output

            Set and cleared by software.

            Bit 0000: MCO output disabled, no clock on MCO
            Bit 0001: SYSCLK system clock selected
            Bit 0010: MSI clock selected.
            Bit 0011: HSI16 clock selected.
            Bit 0100: HSE clock selected
            Bit 0101: Main PLL clock selected
            Bit 0110: LSI clock selected
            Bit 0111: LSE clock selected
            Bit 1000: Internal HSI48 clock selected Others: Reserved

            Note: This clock output may have some truncated cycles at startup or during MCO clock source switching.
        */

        using ClockOutput = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, RCC_CFGR_MCOSEL_Pos>;

        /* Wakeup from Stop and CSS backup clock selection

            Set and cleared by software to select the system clock used when exiting Stop mode.
            The selected clock is also used as emergency clock for the Clock Security System on HSE.

            Warning: STOPWUCK must not be modified when the Clock Security System is enabled by
            HSECSSON in RCC_CR register and the system clock is HSE (SWS=”10”) or a switch on HSE is requested (SW=”10”).

            Bit 0: MSI oscillator selected as wakeup from stop clock and CSS backup clock.
            Bit 1: HSI16 oscillator selected as wakeup from stop clock and CSS backup clock
        */

        using BackupClockSelection = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CFGR_STOPWUCK_Pos>;

        /* APB high-speed prescaler (APB2)

            Set and cleared by software to control the division factor of the APB2 clock (PCLK2).

            Bit 0xx: HCLK not divided
            Bit 100: HCLK divided by 2
            Bit 101: HCLK divided by 4
            Bit 110: HCLK divided by 8
            Bit 111: HCLK divided by 16
        */

        using Apb2HighSpeedPrescaler = mmio::register_mask_type<mmio::field_size_type::u32,  3ul, RCC_CFGR_PPRE1_Pos>;

        /* APB low-speed prescaler (APB1)

            Set and cleared by software to control the division factor of the APB1 clock (PCLK1).

            Bit 0xx: HCLK not divided
            Bit 100: HCLK divided by 2
            Bit 101: HCLK divided by 4
            Bit 110: HCLK divided by 8
            Bit 111: HCLK divided by 16
        */

        using Apb2LowSpeedPrescaler = mmio::register_mask_type<mmio::field_size_type::u32,  3ul, RCC_CFGR_PPRE2_Pos>;

        /* AHB prescaler

            Set and cleared by software to control the division factor of the AHB clock.

            Caution: Depending on the device voltage range, the software has to set correctly these bits to ensure that
            the system frequency does not exceed the maximum allowed frequency (for more details please refer to
            Section 5.1.8: Dynamic voltage scaling management). After a write operation to these bits and before decreasing
            the voltage range, this register must be read to be sure that the new value has been taken into account.

            Bit 0xxx: SYSCLK not divided
            Bit 1000: SYSCLK divided by 2
            Bit 1001: SYSCLK divided by 4
            Bit 1010: SYSCLK divided by 8
            Bit 1011: SYSCLK divided by 16
            Bit 1100: SYSCLK divided by 64
            Bit 1101: SYSCLK divided by 128
            Bit 1110: SYSCLK divided by 256
            Bit 1111: SYSCLK divided by 512
        */

        using AhbPrescaler = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, RCC_CFGR_HPRE_Pos>;

        /* System clock switch status

            Set and cleared by hardware to indicate which clock source is used as system clock.

            Bit 00: MSI oscillator used as system clock
            Bit 01: HSI16 oscillator used as system clock
            Bit 10: HSE used as system clock
            Bit 11: PLL used as system clock
        */

        using SystemClockSwitchStatus = mmio::register_mask_type<mmio::field_size_type::u32,  2ul, RCC_CFGR_SWS_Pos, rcc_system_clock_source_type>;

        /* System clock switch

            Set and cleared by software to select system clock source (SYSCLK).
            Configured by HW to force MSI oscillator selection when exiting Standby or Shutdown mode.
            Configured by HW to force MSI or HSI16 oscillator selection when exiting Stop mode or in
            case of failure of the HSE oscillator, depending on STOPWUCK value.

            Bit 00: MSI oscillator used as system clock
            Bit 01: HSI16 oscillator used as system clock
            Bit 10: HSE used as system clock
            Bit 11: PLL used as system clock
        */

        using SystemClockSwitch = mmio::register_mask_type<mmio::field_size_type::u32,  2ul, RCC_CFGR_SW_Pos, rcc_system_clock_source_type>;

    } ClockConfiguration;

    /*!< RCC system PLL configuration register,                                     Address offset: 0x0C */

    struct SystemPllConfiguration final : mmio::register_type<mmio::field_size_type::u32>
    {
        /* Main PLL division factor for PLLCLK (systemclock).

            Set and cleared by software to control the SAI1 or SAI2 clock frequency.

            PLLSAI3CLK output clock frequency = VCO frequency / PLLPDIV.

            Bit 00000: PLLSAI3CLK is controlled by the bit PLLP
            Bit 00001: Reserved.
            Bit 00010: PLLSAI3CLK = VCO / 2 ....
            Bit 11111: PLLSAI3CLK = VCO / 31
        */

        using MainPllSai3ClockDivisonFactor                  = mmio::register_mask_type<mmio::field_size_type::u32,  5ul, RCC_PLLCFGR_PLLPDIV_Pos>;

        /* Main PLL division factor for PLLCLK (systemclock).

            Set and cleared by software to control the frequency of the main PLL output clock PLLCLK. This output can be selected as system clock.
            These bits can be written only if PLL is disabled.

            PLLCLK output clock frequency = VCO frequency / PLLR with PLLR = 2, 4, 6, or 8

            Bit 00: PLLR = 2
            Bit 01: PLLR = 4
            Bit 10: PLLR = 6
            Bit 11: PLLR = 8
        */

        using MainPllSystemClockDivisonFactor                  = mmio::register_mask_type<mmio::field_size_type::u32,  2ul, RCC_PLLCFGR_PLLR_Pos, rcc_main_pll_system_clock_divison_factor_type>;

        /* Main PLL PLLCLK output enable.

            Set and reset by software to enable the PLLCLK output of the main PLL (used as system clock).
            This bit cannot be written when PLLCLK output of the PLL is used as System Clock.
            In order to save power, when the PLLCLK output of the PLL is not used, the value of PLLREN should be 0.

            Bit 0: PLLCLK output disable
            Bit 1: PLLCLK output enable
        */

        using MainPllClockOutputEnable                       = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_PLLCFGR_PLLREN_Pos>;

        /* Main PLL division factor for PLL48M1CLK (48 MHz clock).

            Set and cleared by software to control the frequency of the main PLL output clock
            PLL48M1CLK. This output can be selected for USB, RNG, SDMMC (48 MHz clock). These bits can be written only if PLL is disabled.

            PLL48M1CLK output clock frequency = VCO frequency / PLLQ with PLLQ = 2, 4, 6, or 8

            Bit 00: PLLQ = 2
            Bit 01: PLLQ = 4
            Bit 10: PLLQ = 6
            Bit 11: PLLQ = 8
        */

        using MainPll48MhzClockDivisionFactor                = mmio::register_mask_type<mmio::field_size_type::u32,  2ul, RCC_PLLCFGR_PLLQ_Pos, rcc_main_pll48_mhz_clock_division_factor_type>;

        /* Main PLL PLL48M1CLK output enable.

            Set and reset by software to enable the PLL48M1CLK output of the main PLL.
            In order to save power, when the PLL48M1CLK output of the PLL is not used, the value of PLLQEN should be 0.

            Bit 0: PLL48M1CLK output disable
            Bit 1: PLL48M1CLK output enable
        */

        using MainPll48MhzClockOutputEnable                   = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_PLLCFGR_PLLQEN_Pos>;

        /* Main PLL division factor for PLLSAI3CLK (SAI1 and SAI2 clock) or SDMMC clock.

            Set and cleared by software to control the frequency of the main PLL output clock PLLSAI3CLK. This output can
            be selected for SAI1 or SAI2 or SDMMC. These bits can be written only if PLL is disabled.
            When the PLLPDIV[4:0] is set to “00000”PLLSAI3CLK output clock frequency = VCO frequency / PLLP with PLLP =7, or 17

            Bit 0: PLLP = 7
            Bit 1: PLLP = 17

            Caution: The software has to set these bits correctly not to exceed 120 MHz on this domain.
        */

        using MainPllDivisionFactorForSai3Clock                   = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_PLLCFGR_PLLP_Pos>;

        /* Main PLL PLLSAI3CLK output enable

            Set and reset by software to enable the PLLSAI3CLK output of the main PLL.
            In order to save power, when the PLLSAI3CLK output of the PLL is not used, the value of PLLPEN should be 0.

            Bit 0: PLLSAI3CLK output disable
            Bit 1: PLLSAI3CLK output enable
        */

        using MainPllSai3ClockOutputEnable                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_PLLCFGR_PLLPEN_Pos>;

        /* Main PLL multiplication factor for VCO

            Set and cleared by software to control the multiplication factor of the VCO. These bits can be written only when the PLL is disabled.
            VCO output frequency = VCO input frequency x PLLN with 8 =< PLLN =< 127

            0000000: PLLN = 0 wrong configuration
            0000001: PLLN = 1 wrong configuration
            ...
            0000111: PLLN = 7 wrong configuration
            0001000: PLLN = 8
            0001001: PLLN = 9
            ...
            1111111: PLLN = 127
        */

        using MainPllMultiplicationFactor                        = mmio::register_mask_type<mmio::field_size_type::u32,  7ul, RCC_PLLCFGR_PLLN_Pos>;

        /* Division factor for the main PLL input clock

            Set and cleared by software to divide the PLL input clock before the VCO. These bits can be written only when all PLLs are disabled.
            VCO input frequency = PLL input clock frequency / PLLM with 1 <= PLLM <= 16

            Bit 0000: PLLM = 1
            Bit 0001: PLLM = 2
            Bit 0010: PLLM = 3
            Bit 0011: PLLM = 4
            Bit 0100: PLLM = 5
            Bit 0101: PLLM = 6
            Bit 0110: PLLM = 7
            Bit 0111: PLLM = 8
        */

        using MainPllInputClockDivisonFactor                        = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, RCC_PLLCFGR_PLLM_Pos, rcc_main_pll_input_clock_division_factor_type>;

        /* Main PLL entry clock source

            Set and cleared by software to select PLL clock source. These bits can be written only when PLL disabled.
            In order to save power, when no PLL is used, the value of PLLSRC should be 00.

            Bit 00: No clock sent to PLL
            Bit 01: MSI clock selected as PLL clock entry
            Bit 10: HSI16 clock selected as PLL clock entry
            Bit 11: HSE clock selected as PLL clock entry
        */

        using MainPllEntryClockSource                        = mmio::register_mask_type<mmio::field_size_type::u32,  2ul, RCC_PLLCFGR_PLLSRC_Pos, rcc_main_pll_entry_clock_source_type>;
    } SystemPllConfiguration;

    mmio::register_type<mmio::field_size_type::u32> PLLSAI1CFGR;                           /*!< RCC PLL SAI1 configuration register,                                       Address offset: 0x10 */
    mmio::register_type<mmio::field_size_type::u32> PLLSAI2CFGR;                           /*!< RCC PLL SAI2 configuration register,                                       Address offset: 0x14 */
    mmio::register_type<mmio::field_size_type::u32> ClockInterruptEnable;                  /*!< RCC clock interrupt enable register,                                       Address offset: 0x18 */
    mmio::register_type<mmio::field_size_type::u32> ClockInterruptFlag;                    /*!< RCC clock interrupt flag register,                                         Address offset: 0x1C */
    mmio::register_type<mmio::field_size_type::u32> ClockInterruptClear;                   /*!< RCC clock interrupt clear register,                                        Address offset: 0x20 */

    mmio::register_type<mmio::field_size_type::u32> Reserved0;                             /*!< Reserved,                                                                  Address offset: 0x24 */

    mmio::register_type<mmio::field_size_type::u32> Clocks[static_cast<std::size_t>(rcc_register_type::k_count)];

    struct ClockControlAndStatus final
    {
        /* Low-power reset flag

            Set by hardware when a reset occurs due to illegal Stop, Standby or Shutdown mode entry. Cleared by writing to the RMVF bit.

            Bit 0: No illegal mode reset occurred
            Bit 1: Illegal mode reset occurred
        */

        using LowPowerResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_LPWRRSTF_Pos>;

        /* Window watchdog reset flag

            Set by hardware when a window watchdog reset occurs. Cleared by writing to the RMVF bit.

            Bit 0: No window watchdog reset occurred
            Bit 1: Window watchdog reset occurred
        */

        using WindowWatchdogResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_WWDGRSTF_Pos>;

        /* Independent window watchdog reset flag

            Set by hardware when an independent watchdog reset domain occurs. Cleared by writing to the RMVF bit.

            Bit 0: No independent watchdog reset occurred
            Bit 1: Independent watchdog reset occurred
        */

        using IndependentWindowWatchdogResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_IWDGRSTF_Pos>;

        /* Softwarereset flag

            Set by hardware when a software reset occurs. Cleared by writing to the RMVF bit.

            Bit 0: No software reset occurred
            Bit 1: Software reset occurred
        */

        using SoftwareResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_SFTRSTF_Pos>;

        /* BOR flag

            Set by hardware when a BOR occurs. Cleared by writing to the RMVF bit.

            Bit 0: No BOR occurred
            Bit 1: BOR occurred
        */

        using BrownOutResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_BORRSTF_Pos>;

        /* Pin reset flag

            Set by hardware when a reset from the NRST pin occurs.
            Cleared by writing to the RMVF bit.

            Bit 0: No reset from NRST pin occurred
            Bit 1: Reset from NRST pin occurred
        */

        using PinResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_PINRSTF_Pos>;

        /* Option byte loader reset flag

            Set by hardware when a reset from the Option Byte loading occurs. Cleared by writing to the RMVF bit.

            Bit 0: No reset from Option Byte loading occurred
            Bit 1: Reset from Option Byte loading occurred
        */

        using OptionByteResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_OBLRSTF_Pos>;

        /* Firewall reset flag

            Set by hardware when a reset from the firewall occurs. Cleared by writing to the RMVF bit.

            Bit 0: No reset from the firewall occurred
            Bit 1: Reset from the firewall occurred
        */

        using FirewallResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_FWRSTF_Pos>;

        /* Remove reset flag

            Set by software to clear the reset flags.

            Bit 0: No effect
            Bit 1: Clear the reset flags
        */

        using RemoveResetFlag                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_RMVF_Pos>;

        /* MSI range after Standby mode

            Set by software to chose the MSI frequency at startup. This range is used after exiting Standby mode until MSIRGSEL is set.
            After a pad or a power-on reset, the range is always 4 MHz. MSISRANGE can be written only when MSIRGSEL = ‘1’.

            Bit 0100: Range 4 around 1 MHz
            Bit 0101: Range 5 around 2 MHz
            Bit 0101: Range 6 around 4 MHz (reset value) TODO: Check if this is correct value
            Bit 0111: Range 7 around 8 MHz
        */

        using MsiRangeAfterStandby                        = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, RCC_CSR_MSISRANGE_Pos, rcc_msi_clock_range_type>;

        /* LSI oscillator ready

            Set and cleared by hardware to indicate when the LSI oscillator is stable.
            After the LSION bit is cleared, LSIRDY goes low after 3 LSI oscillator clock cycles. This bit can be
            set even if LSION = 0 if the LSI is requested by the Clock Security System on LSE, by the Independent Watchdog or by the RTC.

            Bit 0: LSI oscillator not ready
            Bit 1: LSI oscillator ready
        */

        using LowSpeedOscillatorReady                        = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_LSIRDY_Pos>;

        /* LSI oscillator enable

            Set and cleared by software.

            Bit 0: LSI oscillator OFF
            Bit 1: LSI oscillator ON
        */

        using LowSpeedOscillatorEnable                       = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RCC_CSR_LSION_Pos>;
    };

    mmio::register_type<mmio::field_size_type::u32> Reserved8;                             /*!< Reserved,                                                                  Address offset: 0xA0 */
    mmio::register_type<mmio::field_size_type::u32> DLYCFGR;                               /*!< RCC peripheral OCTOSPI delay configuration register,                       Address offset: 0xA4 */
};

static_assert(sizeof(RccLayout) == 168);

} // namespace stm32l4::sys
