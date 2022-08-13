#pragma once

// Includes.

#include <hal.hpp>

namespace sys::hal {

bool board::clock::init()
{
    if ( HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK )
    {
        return false;
    }

    // Configure LSE Drive Capability.

    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    // Initializes the CPU, AHB and APB busses clocks

    static constexpr RCC_OscInitTypeDef k_rcc_oscillator_config =
    {
        .OscillatorType      = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_LSE,
        .HSEState            = 0,
        .LSEState            = RCC_LSE_ON,
        .HSIState            = 0,
        .HSICalibrationValue = 0,
        .LSIState            = RCC_LSI_OFF,
        .MSIState            = RCC_MSI_ON,
        .MSICalibrationValue = 0,
        .MSIClockRange       = RCC_MSIRANGE_6,
        .HSI48State          = 0,
        .PLL =
        {
            .PLLState        = RCC_PLL_ON,
            .PLLSource       = RCC_PLLSOURCE_MSI,
            .PLLM            = 1,
            .PLLN            = 60,
            .PLLP            = RCC_PLLP_DIV5,
            .PLLQ            = RCC_PLLQ_DIV2,
            .PLLR            = RCC_PLLR_DIV2,
        }
    };

    if ( HAL_RCC_OscConfig(const_cast<RCC_OscInitTypeDef*>(std::addressof(k_rcc_oscillator_config))) != HAL_OK )
        return false;

    // Initializes the CPU, AHB and APB busses clocks.

    static constexpr RCC_ClkInitTypeDef k_rcc_clock_config =
    {
        .ClockType      = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2),
        .SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider  = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV1,
        .APB2CLKDivider = RCC_HCLK_DIV1
    };

    if ( HAL_RCC_ClockConfig(const_cast<RCC_ClkInitTypeDef*>(std::addressof(k_rcc_clock_config)), FLASH_LATENCY_5) != HAL_OK )
        return false;

    RCC_PeriphCLKInitTypeDef periph_clk = {};

    periph_clk.PeriphClockSelection = (
        RCC_PERIPHCLK_USART1 |
        RCC_PERIPHCLK_UART4  |
        RCC_PERIPHCLK_OSPI   |
        RCC_PERIPHCLK_RTC    |
        RCC_PERIPHCLK_I2C1   |
        RCC_PERIPHCLK_I2C2   |
        RCC_PERIPHCLK_I2C3   |
        RCC_PERIPHCLK_USB    |
        RCC_PERIPHCLK_SDMMC1
    );

    periph_clk.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    periph_clk.Uart4ClockSelection  = RCC_UART4CLKSOURCE_PCLK1;
    periph_clk.RTCClockSelection    = RCC_RTCCLKSOURCE_LSE;
    periph_clk.I2c1ClockSelection   = RCC_I2C1CLKSOURCE_PCLK1;
    periph_clk.I2c2ClockSelection   = RCC_I2C2CLKSOURCE_PCLK1;
    periph_clk.I2c3ClockSelection   = RCC_I2C3CLKSOURCE_PCLK1;
    periph_clk.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLP;
    periph_clk.UsbClockSelection    = RCC_USBCLKSOURCE_PLLSAI1;
    periph_clk.OspiClockSelection   = RCC_OSPICLKSOURCE_SYSCLK;

    periph_clk.PLLSAI1.PLLSAI1Source   = RCC_PLLSOURCE_MSI;
    periph_clk.PLLSAI1.PLLSAI1M        = 1;
    periph_clk.PLLSAI1.PLLSAI1N        = 24;
    periph_clk.PLLSAI1.PLLSAI1P        = RCC_PLLP_DIV2;
    periph_clk.PLLSAI1.PLLSAI1Q        = RCC_PLLQ_DIV2;
    periph_clk.PLLSAI1.PLLSAI1R        = RCC_PLLR_DIV2;
    periph_clk.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;

    if ( HAL_RCCEx_PeriphCLKConfig(std::addressof(periph_clk)) != HAL_OK )
        return false;

    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

    // Enables the Clock Security System.

    // HAL_RCCEx_EnableLSECSS();

    // Enable MSI Auto calibration.

    HAL_RCCEx_EnableMSIPLLMode();

    return true;
}

} // namespace sys::hal
