#pragma once

namespace sys::arch::arm {

enum class vendor_irqn_type : std::int32_t;

enum class core_irqn_type : std::int32_t
{
    k_reset_irq                 = -15,
    k_non_maskable_irq          = -14,    /*!< 2 Cortex-M4 Non Maskable Interrupt                                */
    k_hard_fault_irq            = -13,    /*!< 3 Cortex-M4 Hard Fault Interrupt                                  */
    k_memory_managment_irq      = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
    k_bus_fault_irq             = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
    k_usage_fault_irq           = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
    k_service_call_irq          = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
    k_debug_monitor_irq         = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
    k_pending_service_call_irq  = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
    k_system_tick_irq           = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
};

enum class irq_priority_type : std::uint32_t
{
    k_level_0 = 0,
    k_level_1,
    k_level_2,
    k_level_3,
    k_level_4,
    k_level_5,
    k_level_6,
    k_level_7,
    k_level_8,
    k_level_9,
    k_level_10,
    k_level_11,
    k_level_12,
    k_level_13,
    k_level_14,
    k_level_15,
    k_none,
};

} // namespace sys::arch::arm

namespace sys::hal {

// Declaration for the interrupt vector address type.

using interrupt_handler_type = void (*)(void);

constexpr std::tuple<device_node, arch::arm::core_irqn_type, arch::arm::irq_priority_type> k_core_interrupt_table[] =
{
    { device_node{"/chip/cpu@0"},             arch::arm::core_irqn_type::k_reset_irq,                arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cpu@0"},             arch::arm::core_irqn_type::k_non_maskable_irq,         arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cpu@0"},             arch::arm::core_irqn_type::k_hard_fault_irq,           arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cpu@0"},             arch::arm::core_irqn_type::k_memory_managment_irq,     arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cpu@0"},             arch::arm::core_irqn_type::k_bus_fault_irq,            arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cpu@0"},             arch::arm::core_irqn_type::k_usage_fault_irq,          arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cortex-m-dwt"},      arch::arm::core_irqn_type::k_debug_monitor_irq,        arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cpu@0"},             arch::arm::core_irqn_type::k_service_call_irq,         arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cpu@0"},             arch::arm::core_irqn_type::k_pending_service_call_irq, arch::arm::irq_priority_type::k_none    },
    { device_node{"/chip/cortex-m-systick"},  arch::arm::core_irqn_type::k_system_tick_irq,          arch::arm::irq_priority_type::k_level_0 },
};

} // namespace sys::hal
