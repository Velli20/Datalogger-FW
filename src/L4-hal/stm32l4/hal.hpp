#pragma once

// Target specific definations.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <stm32l4xx_hal.h>
#pragma GCC diagnostic pop

// External C function declarations.

extern "C" void SystemInit(void);

// External C-variable declarations.

extern "C" std::uint32_t SystemCoreClock;

// HAL-types.

#include <types/hal_board_type.hpp>

#include <jedec/jedec_cfi_command.hpp>
#include <jedec/jedec_sfdp_header.hpp>
#include <jedec/jedec_parameter_table.hpp>

#include <stm32l4/detail/stm32l4_instance_of_detail.hpp>
#include <stm32l4/concepts/stm32l4_concepts.hpp>

#include <stm32l4/types/stm32l4_uart_error_category.hpp>
#include <stm32l4/types/stm32l4_hal_status_error_category_types.hpp>
#include <stm32l4/types/stm32l4_hal_sdmmc_error_category.hpp>
#include <stm32l4/types/stm32l4_hal_i2c_error_category.hpp>
#include <stm32l4/types/stm32l4_hal_ospi_error_category.hpp>
#include <stm32l4/types/stm32l4_hal_ospi_command_types.hpp>
#include <stm32l4/types/stm32l4_hal_handle_types.hpp>

#include <stm32l4/types/stm32l4_rcc_types.hpp>
#include <stm32l4/types/stm32l4_exti_types.hpp>
#include <stm32l4/types/stm32l4_gpio_types.hpp>
#include <stm32l4/types/stm32l4_uart_types.hpp>
#include <stm32l4/types/stm32l4_rtc_types.hpp>
#include <stm32l4/types/stm32l4_dma_types.hpp>
#include <stm32l4/types/stm32l4_sdmmc_types.hpp>
#include <stm32l4/types/stm32l4_ospi_types.hpp>
#include <stm32l4/types/stm32l4_power_types.hpp>
#include <stm32l4/types/stm32l4_i2c_types.hpp>
#include <stm32l4/types/stm32l4_usb_types.hpp>

#include <stm32l4/bits/stm32l4_rcc_bits.hpp>
#include <stm32l4/bits/stm32l4_exti_bits.hpp>
#include <stm32l4/bits/stm32l4_gpio_bits.hpp>
#include <stm32l4/bits/stm32l4_uart_bits.hpp>
#include <stm32l4/bits/stm32l4_rtc_bits.hpp>
#include <stm32l4/bits/stm32l4_dma_bits.hpp>
#include <stm32l4/bits/stm32l4_power_bits.hpp>

#include <cortex-m/types/cortex_m_systick_types.hpp> // FIXME

#include <vendor_irqn_type.hpp>
#include <interrupt_map.hpp>
#include <pin_tree.hpp>
#include <peripheral_tree.hpp>

// Arch.

#include <arch.hpp>

// OS.

#include <operating-system.hpp>

// STM32L4-specific includes.

#include <stm32l4/stm32l4_rcc.hpp>
#include <stm32l4/stm32l4_power_control.hpp>
#include <stm32l4/stm32l4_rtc.hpp>
#include <stm32l4/stm32l4_system_configuration_controller.hpp>
#include <stm32l4/stm32l4_exti.hpp>
#include <stm32l4/stm32l4_gpio.hpp>
#include <stm32l4/stm32l4_dma.hpp>
#include <stm32l4/stm32l4_uart.hpp>
#include <stm32l4/stm32l4_i2c.hpp>
#include <stm32l4/stm32l4_sdmmc.hpp>
#include <stm32l4/stm32l4_usb.hpp>
#include <stm32l4/stm32l4_octospi.hpp>
