#pragma once

namespace sys::hal {

/* PM0214: Page 221 */
struct SystemControlBlockLayout
{
    /*!< Offset: 0x000 (R/ )  CPUID Base Register */

    const union
    {
        /* Indicates implementor: 0x41 = ARM */

        mmio::bitmask_type<mmio::field_size_type::u32,  8ul, SCB_CPUID_IMPLEMENTER_Pos>   Implementer;

        /* Indicates processor revision: 0x0 = Revision 0 */

        mmio::bitmask_type<mmio::field_size_type::u32,  4ul, SCB_CPUID_VARIANT_Pos>       Variant;

        /* Reads as 0xF */

        mmio::bitmask_type<mmio::field_size_type::u32,  4ul, SCB_CPUID_ARCHITECTURE_Pos>  Architecture;

        /* Indicates part number: 0xC24 = Cortex-M4 */

        mmio::bitmask_type<mmio::field_size_type::u32, 12ul, SCB_CPUID_PARTNO_Pos>        PartNumber;

        /* Indicates patch release: 0x0 = Patch 0. */

        mmio::bitmask_type<mmio::field_size_type::u32,  4ul, SCB_CPUID_REVISION_Pos>      Revision;
    } CpuId;

    /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */

    union
    {
        /* NMI set-pending bit:

            Write:

            Bit 0: No effect
            Bit 1: Change NMI exception state to pending.

            Read:

            Bit 0: NMI exception is not pending
            Bit 1: NMI exception is pending
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_ICSR_NMIPENDSET_Pos>   NonMaskableInterruptSet;

        /* PendSV set-pending bit:

            Write:

            Bit 0: No effect.
            Bit 1: Change PendSV exception state to pending.

            Read:

            Bit 0: PendSV exception is not pending.
            Bit 1: PendSV exception is pending.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_ICSR_PENDSVSET_Pos>    PendableServiceInterruptSet;

        /* PendSV clear-pending bit. This bit is write-only. On a read, value is unknown:

            Write:

            Bit 0: No effect.
            Bit 1: Removes the pending state from the PendSV exception.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_ICSR_PENDSVCLR_Pos>    PendableServiceInterruptClear;

        /* SysTick exception set-pending bit:

            Write:

            Bit 0: No effect.
            Bit 1: Change SysTick exception state to pending.

            Read:

            Bit 0: SysTick exception is not pending.
            Bit 1: SysTick exception is pending.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_ICSR_PENDSTSET_Pos>    SystickInterruptSet;

        /* SysTick exception cleat-pending bit. This bit is write-only. On a read, value is unknown:

            Write:

            Bit 0: No effect.
            Bit 1: Removes the pending state from the SysTick exception.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_ICSR_PENDSTCLR_Pos>    SystickInterruptClear;

        /* This bit is reserved for Debug use and reads-as-zero when the processor is not in Debug:

            Read:

            Bit 0: TODO.
            Bit 1: TODO.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_ICSR_ISRPREEMPT_Pos>   InterruptPreempt;

        /* Interrupt pending flag, excluding NMI and Faults:

            Read:

            Bit 0: Interrupt not pending.
            Bit 1: Interrupt pending.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_ICSR_ISRPENDING_Pos>   InterruptPending;

        /* Pending vector. Indicates the exception number of the highest priority pending enabled exception:

            Read:

            Bit 0:        No pending exceptions
            Other values: The exception number of the highest priority pending enabled exception.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  7ul, SCB_ICSR_VECTPENDING_Pos>  PendingVector;

        /* Return to base level. Indicates whether there are preempted active exceptions:

            Read:

            Bit 0: There are preempted active exceptions to execute
            Bit 1: There are no active exceptions, or the currently-executing exception is the only active exception.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_ICSR_RETTOBASE_Pos>    ReturnToBaseLevel;

        /* Active vector. Contains the active exception number:

            Read:

            Bit 0:        Thread mode:
            Other values: The exception number (*) of the currently active exception.
            (*):          Subtract 16 from this value to obtain CMSIS IRQ number required to index into the Interrupt Clear-Enable,
                          Set-Enable, Clear-Pending, Set-Pending, or Priority Registers.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  9ul, SCB_ICSR_VECTACTIVE_Pos>    ActiveVector;
    } InterruptControlAndState;

    /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */

    union
    {
        /* Vector table base offset field:

            Contains bits [29:9] of the offset of the table base from memory address 0x00000000.
            When setting TBLOFF, you must align the offset to the number of exception entries in the vector table.
            The minimum alignment is 128 words. Table alignment requirements mean that bits[8:0] of the table offset are always zero.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  9ul, SCB_VTOR_TBLOFF_Pos>        Offset;

        /* Vector table in location. Determines whether the vector table is in the code or SRAM memory region:

            Bit 0: Code
            Bit 1: SRAM
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_VTOR_TBLOFF_Pos>        Location;
    } VectorTableOffset;

    /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control mmio::register_type. Required privilege: Privileged */

    mmio::register_type<mmio::field_size_type::u32>       AppInterruptAndResetControl;

    /*!< Offset: 0x010 (R/W)  System Control Register */

    mmio::register_type<mmio::field_size_type::u32>       SystemControl;

    /*!< Offset: 0x014 (R/W)  Configuration Control Register */

    union
    {
        /* STKALIGN:

            Configures stack alignment on exception entry. On exception entry, the processor uses bit 9 of the stacked PSR to indicate the stack alignment.
            On return from the exception it uses this stacked bit to restore the correct stack alignment.

                Bit 0: 4-byte aligned
                Bit 1: 8-byte aligned
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_CCR_STKALIGN_Pos>       stack_alignment_type;

        /* BFHFNMIGN:

            Enables handlers with priority -1 or -2 to ignore data bus faults caused by load and store instructions.
            This applies to the hard fault, NMI, and FAULTMASK escalated handlers. Set this bit to 1 only when the
            handler and its data are in absolutely safe memory. The normal use of this bit is to probe system
            devices and bridges to detect control path problems and fix them.

                Bit 0: Data bus faults caused by load and store instructions cause a lock-up
                Bit 1: Handlers running at priority -1 and -2 ignore data bus faults caused by load and store
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_CCR_BFHFNMIGN_Pos>      BusFaultHandlerIgnore;

        /* DIV_0_TRP:

            Enables faulting or halting when the processor executes an SDIV or UDIV instruction with a divisor of 0:

                Bit 0: Do not trap divide by 0.
                Bit 1: Trap divide by 0.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_CCR_DIV_0_TRP_Pos>      DivideByZeroTrap;

        /* UNALIGN_TRP:

            Enables unaligned access traps:

                Bit 0: Do not trap unaligned halfword and word accesses.
                Bit 1: Trap unaligned halfword and word accesses.

            If this bit is set to 1, an unaligned access generates a usage fault.
            Unaligned LDM, STM, LDRD, and STRD instructions always fault irrespective of whether UNALIGN_TRP is set to 1.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_CCR_UNALIGN_TRP_Pos>    UnalignedAccessTrap;

        /* USERSETMPEND:

            Enables unprivileged software access to the STIR, see Software trigger interrupt register:

                Bit 0: Disable.
                Bit 1: Enable.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_CCR_USERSETMPEND_Pos>   UnprivilegedSoftwareInterruptTrigger;

        /* NONBASETHRDENA:

            Configures how the processor enters Thread mode.

                Bit 0: Processor can enter Thread mode only when no exception is active.
                Bit 1: Processor can enter Thread mode from any level under the control of an EXC_RETURN.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_CCR_NONBASETHRDENA_Pos> ThreadModeEntry;

    } ConfigurationControl;

    /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */

    mmio::register_type<mmio::field_size_type::u8>        SystemHandlersPriority[12U];

    /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */

    union
    {
        /* USGFAULTENA: Usage fault enable bit, set to 1 to enable */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_USGFAULTENA_Pos>    UsageFault;

        /* BUSFAULTENA: Bus fault enable bit, set to 1 to enable */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_BUSFAULTENA_Pos>    BusFault;

        /* MEMFAULTENA: Memory managment fault enable bit, set to 1 to enable */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_MEMFAULTENA_Pos>    MemoryManagmentFault;

        /* SVCALLPENDED: SVC call pending bit, reads as 1 if exception is pending */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_SVCALLPENDED_Pos>   ServiceCallPending;

        /* BUSFAULTPENDED: Bus fault exception pending bit, reads as 1 if exception is pending */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_BUSFAULTPENDED_Pos> BusFaultPending;

        /* MEMFAULTPENDED: Memory managemnet fault exception pending bit, reads as 1 if exception is pending */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_MEMFAULTPENDED_Pos> MemoryManagmentFaultPending;

        /* USGFAULTPENDED: Usage fault exception pending bit, reads as 1 if exception is pending */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_USGFAULTPENDED_Pos> UsageFaultPending;

        /* SYSTICKACT: SysTick exception active bit, reads as 1 if exception is active */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_SYSTICKACT_Pos>     SysTickActive;

        /* PENDSVACT: PendSV exception active bit, reads as 1 if exception is active */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_PENDSVACT_Pos>      PendingServiceCallActive;

        /* MONITORACT: Debug monitor active bit, reads as 1 if Debug monitor is active */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_MONITORACT_Pos>     DebugMonitorActive;

        /* SVCALLACT: SVC call active bit, reads as 1 if SVC call is active */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_SVCALLACT_Pos>      ServiceCallActive;

        /* USGFAULTACT: Usage fault exception active bit, reads as 1 if exception is active */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_USGFAULTACT_Pos>    UsageFaultActive;

        /* BUSFAULTACT: Bus fault exception active bit, reads as 1 if exception is active */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_BUSFAULTACT_Pos>    BusFaultActive;

        /* MEMFAULTACT: Memory managment fault exception active bit, reads as 1 if exception is active */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_SHCSR_MEMFAULTACT_Pos>    MemoryManagmentFaultActive;

    } SystemHandlerControlAndState;

    /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register CFSR; UFSR+BFSR+MMFSR */

    mmio::register_type<mmio::field_size_type::u32>                                            ConfigurableFaultStatus;

    /*!< Offset: 0x02C (R/W)  HardFault Status Register */

    union
    {
        /* DEBUG_VT */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_HFSR_DEBUGEVT_Pos>        DebugEvent;

        /* FORCED */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_HFSR_FORCED_Pos>          ForcedHardFault;

        /* VECTTBL */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCB_HFSR_VECTTBL_Pos>         VectorTableHardFault;
    } HardFaultStatus;

    /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */

    mmio::register_type<mmio::field_size_type::u32>       DebugFaultStatus;

    /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */

    mmio::register_type<mmio::field_size_type::u32>       MemManageFaultAddress;

    /*!< Offset: 0x038 (R/W)  BusFault Address Register */

    mmio::register_type<mmio::field_size_type::u32>       BusFaultAddress;

    /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */

    mmio::register_type<mmio::field_size_type::u32>       AuxiliaryFaultStatus;

    /*!< Offset: 0x040 (R/ )  Processor Feature Register */

    const mmio::register_type<mmio::field_size_type::u32> ProcessorFeature[2U];

    /*!< Offset: 0x048 (R/ )  Debug Feature Register */

    const mmio::register_type<mmio::field_size_type::u32> DebugFeature;

    /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register */

    const mmio::register_type<mmio::field_size_type::u32> AuxiliaryFeature;

    /*!< Offset: 0x050 (R/ )  Memory Model Feature Register */

    const mmio::register_type<mmio::field_size_type::u32> MemoryModelFeature[4U];

    /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register */

    const mmio::register_type<mmio::field_size_type::u32> InstructionSetAttributes[5U];


    const mmio::register_type<mmio::field_size_type::u32> Reserved[5U];

    /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */

    union
    {
        /* NONBASETHRDENA:

            Access privileges for coprocessor <N>. The possible values of each field are:

                Bit 0b00: Access denied. Any attempted access generates a NOCP UsageFault.
                Bit 0b01: Privileged access only. An unprivileged access generates a NOCP fault.
                Bit 0b10: Reserved. The result of any access is Unpredictable.
                Bit 0b11: Full access.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, 20ul>                       Coprocessor10Access;
        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, 22u>                        Coprocessor11Access;

    } CoprocessorAccessControl;
};

static_assert(sizeof(SystemControlBlockLayout) == sizeof(SCB_Type));

/* PM0214: Page 253 */

struct FloatingPointUnitLayout
{
    const mmio::register_type<mmio::field_size_type::u32> Reserved0;

    /*!< Offset: 0x004 (R/W)  Floating-Point Context Control Register */

    __IOM uint32_t FPCCR;

    /*!< Offset: 0x008 (R/W)  Floating-Point Context Address Register */

    __IOM uint32_t FPCAR;

    /*!< Offset: 0x00C (R/W)  Floating-Point Default Status Control Register */

    __IOM uint32_t FPDSCR;

    /*!< Offset: 0x010 (R/ )  Media and FP Feature mmio::register_type 0 */

    __IM  uint32_t MVFR0;

    /*!< Offset: 0x014 (R/ )  Media and FP Feature mmio::register_type 1 */

    __IM  uint32_t MVFR1;
};

static_assert(sizeof(FloatingPointUnitLayout) == sizeof(FPU_Type));

struct SystemControlsAndIdLayout
{
    mmio::register_type<mmio::field_size_type::u32>       Reserved;

    /*!< Offset: 0x004 (R/ )  Interrupt Controller Type Register */

    const mmio::register_type<mmio::field_size_type::u32> InterruptControllerType;

    /*!< Offset: 0x008 (R/W)  Auxiliary Control Register */

    union
    {
        /* Disables lazy stacking of floating point context.*/

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCnSCB_ACTLR_DISOOFP_Pos>    DisableFloatLazyStacking;

        /* Disables floating point instructions completing out of order with respect to integer instructions. */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCnSCB_ACTLR_DISFPCA_Pos>    DisableFloatInstructionsReorder;

        /* Disables write buffer use during default memory map accesses. This causes all bus faults to be
           precise, but decreases the performance of the processor because stores to memory must
           complete before the next instruction can be executed.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCnSCB_ACTLR_DISDEFWBUF_Pos> DisableWriteBuffer;

        /* Disables interruption of multi-cycle instructions. This increases the interrupt latency of
           the processor because load/store and multiply/divide operations complete before interrupt stacking occurs.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, SCnSCB_ACTLR_DISMCYCINT_Pos> DisableMultiCycleInstructionInterrupt;

        /* Disables folding of IT instructions. */

        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, SCnSCB_ACTLR_DISFOLD_Pos>    DisableFoldingOfItInstruction;

    } AuxiliaryControl;
};

static_assert(sizeof(SystemControlsAndIdLayout) == sizeof(SCnSCB_Type));

} // namespace sys::hal
