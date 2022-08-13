#pragma once

#include <cortex-m/types/cortex_m_interrupt_types.hpp>

namespace sys::hal {

constexpr std::tuple<device_node, arch::arm::vendor_irqn_type, arch::arm::irq_priority_type> k_interrupt_map[] =
{
    { device_node{"/chip/dma1-channel@5"},  arch::arm::vendor_irqn_type::k_dma1_channel5,       arch::arm::irq_priority_type::k_level_4  },
    { device_node{"/chip/usart@1"},         arch::arm::vendor_irqn_type::k_usart_global1,       arch::arm::irq_priority_type::k_level_3  },
    { device_node{"/chip/uart@4"},          arch::arm::vendor_irqn_type::k_uart_global4,        arch::arm::irq_priority_type::k_level_2  },
    { device_node{"/chip/dma2-channel@1"},  arch::arm::vendor_irqn_type::k_dma2_channel1,       arch::arm::irq_priority_type::k_level_10 },
    { device_node{"/chip/i2c@1"},           arch::arm::vendor_irqn_type::k_i2c1_error,          arch::arm::irq_priority_type::k_level_6  },
    { device_node{"/chip/i2c@1"},           arch::arm::vendor_irqn_type::k_i2c1_event,          arch::arm::irq_priority_type::k_level_6  },
    { device_node{"/chip/i2c@2"},           arch::arm::vendor_irqn_type::k_i2c2_error,          arch::arm::irq_priority_type::k_level_7  },
    { device_node{"/chip/i2c@2"},           arch::arm::vendor_irqn_type::k_i2c2_event,          arch::arm::irq_priority_type::k_level_7  },
    { device_node{"/chip/sdmmc@1"},         arch::arm::vendor_irqn_type::k_sdmmc_global,        arch::arm::irq_priority_type::k_level_5  },
    { device_node{"/chip/exti"},            arch::arm::vendor_irqn_type::k_external_line0,      arch::arm::irq_priority_type::k_level_3  },
    { device_node{"/chip/exti"},            arch::arm::vendor_irqn_type::k_external_line1,      arch::arm::irq_priority_type::k_level_3  },
    { device_node{"/chip/exti"},            arch::arm::vendor_irqn_type::k_external_line2,      arch::arm::irq_priority_type::k_level_3  },
    { device_node{"/chip/exti"},            arch::arm::vendor_irqn_type::k_external_line3,      arch::arm::irq_priority_type::k_level_3  },
    { device_node{"/chip/usb-otg"},         arch::arm::vendor_irqn_type::k_otg_filesystem,      arch::arm::irq_priority_type::k_level_7  },
    { device_node{"/chip/exti"},            arch::arm::vendor_irqn_type::k_external_line4,      arch::arm::irq_priority_type::k_level_3  },
    { device_node{"/chip/exti"},            arch::arm::vendor_irqn_type::k_external_line9to5,   arch::arm::irq_priority_type::k_level_3  },
    { device_node{"/chip/exti"},            arch::arm::vendor_irqn_type::k_external_line15to10, arch::arm::irq_priority_type::k_level_3  },
};

} // namespace sys::hal
