#pragma once

namespace sys::hal {

struct DataWatchPointAndTraceLayout
{
    /*!< Offset: 0x000 (R/W)  Control Register */

    struct Control final : public mmio::register_type<mmio::field_size_type::u32>
    {
        using CycleCounterEnable                    = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_CYCCNTENA_Pos>;
        using PostCounterReloadValue                = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, DWT_CTRL_POSTPRESET_Pos>;
        using PostCounterInitialValue               = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, DWT_CTRL_POSTINIT_Pos>;
        using PostCounterTapValue                   = mmio::register_mask_type<mmio::field_size_type::u32,  2ul, DWT_CTRL_CYCTAP_Pos>;
        using PostCounterEnable                     = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_PCSAMPLENA_Pos>;
        using ExceptionTraceEnable                  = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_EXCTRCENA_Pos>;
        using CpiCounterOverflowEvent               = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_CPIEVTENA_Pos>;
        using ExceptionOverheadCounterOverflowEvent = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_EXCEVTENA_Pos>;
        using SleepCounterOverflowEvent             = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_SLEEPEVTENA_Pos>;
        using LsuCounterOverflowEvent               = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_LSUEVTENA_Pos>;
        using FoldedInstructionCounterOverflowEvent = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_FOLDEVTENA_Pos>;
        using PostCounterUnderflowEvent             = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_CYCEVTENA_Pos>;
        using ProfilingCountersSupported            = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_NOPRFCNT_Pos>;
        using CycleCounterSupported                 = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_NOCYCCNT_Pos>;
        using ExternalMachSignalsSupported          = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_NOEXTTRIG_Pos>;
        using TraceSamplingSupported                = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_CTRL_NOTRCPKT_Pos>;
        using ComparatorCount                       = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, DWT_CTRL_NUMCOMP_Pos>;
    } Control;

    /*!< Offset: 0x004 (R/W)  Cycle Count Register */

    mmio::register_type<mmio::field_size_type::u32>  CycleCounter;

    /*!< Offset: 0x008 (R/W)  CPI Count Register */

    mmio::register_type<mmio::field_size_type::u32> CpiCount;

    /*!< Offset: 0x00C (R/W)  Exception Overhead Count Register */

    mmio::register_type<mmio::field_size_type::u32> ExceptionOverheadCount;

    /*!< Offset: 0x010 (R/W)  Sleep Count Register */

    mmio::register_type<mmio::field_size_type::u32> SleepCount;

    /*!< Offset: 0x014 (R/W)  LSU Count Register */

    mmio::register_type<mmio::field_size_type::u32> LsuCount;

    /*!< Offset: 0x018 (R/W)  Folded-instruction Count Register */

    mmio::register_type<mmio::field_size_type::u32> FoldedInstructionsCount;

    /*!< Offset: 0x01C (R/ )  Program Counter Sample Register */

    mmio::register_type<mmio::field_size_type::u32> ProgramCounterSample;

    struct FunctionField final : public mmio::register_type<mmio::field_size_type::u32>
    {
        using Matched                                      = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_FUNCTION_MATCHED_Pos>;
        using DataVaAddress1                               = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, DWT_FUNCTION_DATAVADDR1_Pos>;
        using DataVaAddress0                               = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, DWT_FUNCTION_DATAVADDR0_Pos>;
        using DataVaSize                                   = mmio::register_mask_type<mmio::field_size_type::u32,  2ul, DWT_FUNCTION_DATAVADDR0_Pos>;
        using DataValueComparisonEnable                    = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_FUNCTION_DATAVMATCH_Pos>;
        using ComparatorZeroCycleCountComparisonEnable     = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_FUNCTION_CYCMATCH_Pos>;
        using DataTraceEnable                              = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DWT_FUNCTION_EMITRANGE_Pos>;
        using Action                                       = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, DWT_FUNCTION_FUNCTION_Pos, data_watch_point_function_type>;
    };

    /* C1-746 */
    struct WatchPoint final
    {
        /*!< Offset: 0x020 (R/W)  Comparator mmio::register_type 0 */

        mmio::register_type<mmio::field_size_type::u32>                                    Comparator;

        /*!< Offset: 0x024 (R/W)  Mask Register */

        mmio::register_type<mmio::field_size_type::u32>                                    Mask;

        /*!< Offset: 0x028 (R/W)  Function Register */

        FunctionField                                               Function;

        /**/
        const mmio::register_type<mmio::field_size_type::u32>                              Reserved;
    } WatchPoint[4];
};

static_assert(sizeof(DataWatchPointAndTraceLayout) == (92 + sizeof(mmio::register_type<mmio::field_size_type::u32>)));

struct CoreDebugLayout
{
    mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul> HaltingControlAndStatusRegister;      /*!< Offset: 0x000 (R/W)  Debug Halting Control and Status Register */
    const mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul> Selector;                       /*!< Offset: 0x004 ( /W)  Debug Core mmio::register_type Selector Register */
    mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul> Data;                                 /*!< Offset: 0x008 (R/W)  Debug Core mmio::register_type Data Register */

    /*!< Offset: 0x00C (R/W)  Debug Exception and Monitor Control Register */

    struct DebugAndMonitorCtrl final : public mmio::register_type<mmio::field_size_type::u32>
    {
        using TraceEnable = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, CoreDebug_DEMCR_TRCENA_Pos>;
    } DebugAndMonitorCtrl;
};

static_assert(sizeof(CoreDebugLayout) == 16);

} // namespace sys::hal
