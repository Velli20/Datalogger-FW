#pragma once

namespace sys::hal {

struct SysTickLayout
{
    /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */

    struct Control final : mmio::register_type<mmio::field_size_type::u32>
    {
        using CountFlag              = mmio::register_mask_type<mmio::field_size_type::u32, 1ul, SysTick_CTRL_COUNTFLAG_Pos>;
        using Source                 = mmio::register_mask_type<mmio::field_size_type::u32, 1ul, SysTick_CTRL_CLKSOURCE_Pos, sys_tick_source_type>;
        using ExecptionRequestEnable = mmio::register_mask_type<mmio::field_size_type::u32, 1ul, SysTick_CTRL_TICKINT_Pos>;
        using CounterEnable          = mmio::register_mask_type<mmio::field_size_type::u32, 1ul, SysTick_CTRL_ENABLE_Pos>;
    } Control;

    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */

    mmio::bitmask_type<mmio::field_size_type::u32, 24ul, SysTick_LOAD_RELOAD_Pos>                       ReloadValue;

    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */

    mmio::bitmask_type<mmio::field_size_type::u32, 24ul, SysTick_VAL_CURRENT_Pos>                       CurrentValue;

    /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */

    const mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                                     Calibration;
};

static_assert(sizeof(SysTickLayout) == 16);

} // namespace sys::hal
