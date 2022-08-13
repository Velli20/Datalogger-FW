#pragma once

namespace sys::hal {

/* Page 360 */
struct SystemConfigurationLayout
{
    /*!< SYSCFG memory remap register,                      Address offset: 0x00      */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_MEMRMP_FB_MODE_Pos>   FlashBankMode;
        mmio::bitmask_type<mmio::field_size_type::u32, 3ul, SYSCFG_MEMRMP_MEM_MODE_Pos>  MemoryMappingSelection;
    } MemoryRemap;

    /*!< SYSCFG configuration register 1,                   Address offset: 0x04      */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, 31ul>                        FloatingPointInexactInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, 30ul>                        InputDenormalInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, 29ul>                        OverflowInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, 28ul>                        UnderflowInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, 27ul>                        DivideByZeroInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, 26ul>                        InvalidOperationInterrupt;

        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR1_I2C4_FMP_Pos>   I2c4FastModePlusActivation;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR1_I2C3_FMP_Pos>   I2c3FastModePlusActivation;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR1_I2C2_FMP_Pos>   I2c2FastModePlusActivation;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR1_I2C1_FMP_Pos>   I2c1FastModePlusActivation;
    } Configuration1;

    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */

    /* EXTI X (= kLine) configuration bits:
        Bit 0000: PA[X] pin
        Bit 0001: PB[X] pin
        Bit 0010: PC[X] pin
        Bit 0011: PD[X] pin
        Bit 0100: PE[X] pin
        Bit 0101: PF[X] pin
        Bit 0110: PG[X] pin
        Bit 0111: PH[X] pin
        Bit 1000: PI[X] pin
    */

    private:

    mmio::register_type<mmio::field_size_type::u32>                                          ExternalInterruptConfiguration1;
    mmio::register_type<mmio::field_size_type::u32>                                          ExternalInterruptConfiguration2;
    mmio::register_type<mmio::field_size_type::u32>                                          ExternalInterruptConfiguration3;
    mmio::register_type<mmio::field_size_type::u32>                                          ExternalInterruptConfiguration4;

    public:

    template <std::uint32_t kLine>
    constexpr auto ExternalInterruptConfiguration() noexcept
    {
        constexpr std::uint32_t kOffset = ((kLine % 4ul) * 4ul);

        if constexpr ( kLine < 4 )
        {
            return ExternalInterruptConfiguration1.Field<4ul, kOffset>();
        }

        else if constexpr ( kLine < 8 )
        {
            return ExternalInterruptConfiguration2.Field<4ul, kOffset>();
        }

        else if constexpr ( kLine < 12 )
        {
            return ExternalInterruptConfiguration3.Field<4ul, kOffset>();
        }

        else if constexpr ( kLine < 16 )
        {
            return ExternalInterruptConfiguration4.Field<4ul, kOffset>();
        }
    }

    /*!< SYSCFG SRAM2 control and status register,          Address offset: 0x18      */

    union
    {
        /* SRAM2 busy by erase operation:
            Bit 0: No SRAM2 erase operation is on going.
            Bit 1: SRAM2 erase operation is on going.
        */
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_SCSR_SRAM2BSY_Pos>    EraseOngoing;

        /* SRAM2 Erase:
            Setting this bit starts a hardware SRAM2 erase operation. This bit is
            automatically cleared at the end of the SRAM2 erase operation.
            Note: This bit is write-protected: setting this bit is possible only after
            the correct key sequence is written in the SYSCFG_SKR register.
        */
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_SCSR_SRAM2ER_Pos>     Erase;

    } SramControlAndStatus;

    /*!< SYSCFG configuration register 2,                   Address offset: 0x1C      */

    union
    {
        /* SRAM2 parity error flag:
            This bit is set by hardware when an SRAM2 parity error is detected. It is cleared by software by writing ‘1’.

            Bit 0: No SRAM2 parity error detected
            Bit 1: SRAM2 parity error detected
        */
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR2_SPF_Pos>        ParityError;

        /* ECC Lock:
            This bit is set by software and cleared only by a system reset. It can be used to enable and lock the
            Flash ECC error connection to TIM1/8/15/16/17 Break input.

            Bit 0: ECC error disconnected from TIM1/8/15/16/17 Break input.
            Bit 1: ECC error connected to TIM1/8/15/16/17 Break input.
        */
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR2_ECCL_Pos>       EccLock;

        /* PVD Lock:
            This bit is set by software and cleared only by a system reset. It can be used to
            enable and lock the PVD connection to TIM1/8/15/16/17 Break input, as well as the PVDE and PLS[2:0] in the PWR_CR2 register.

            Bit 0: PVD interrupt disconnected from TIM1/8/15/16/17 Break input. PVDE and PLS[2:0] bits can be programmed by the application.
            Bit 1: PVD interrupt connected to TIM1/8/15/16/17 Break input, PVDE and PLS[2:0] bits are read only.
        */
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR2_PVDL_Pos>       PvdLock;

        /* SRAM2 parity lock bit:
            This bit is set by software and cleared only by a system reset. It can be used to
            enable and lock the SRAM2 parity error signal connection to TIM1/8/15/16/17 Break inputs.

            Bit 0: SRAM2 parity error signal disconnected from TIM1/8/15/16/17 Break inputs
            Bit 1: SRAM2 parity error signal connected to TIM1/8/15/16/17 Break inputs
        */
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR2_SPL_Pos>        ParityLock;

        /* Cortex®-M4LOCKUP (Hardfault) output enable bit:
            This bit is set by software and cleared only by a system reset. It can be used to
            enable and lock the connection of Cortex®-M4 LOCKUP (Hardfault) output to TIM1/8/15/16/17 Break input

            Bit 0: Cortex®-M4 LOCKUP output disconnected from TIM1/8/15/16/17 Break inputs
            Bit 1: Cortex®-M4 LOCKUP output connected to TIM1/8/15/16/17 Break inputs
        */
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, SYSCFG_CFGR2_CLL_Pos>        CoreLockupLock;
    } Configuration2;

    /*!< SYSCFG SRAM2 write protection register,            Address offset: 0x20      */

    mmio::register_type<mmio::field_size_type::u32>                            SramWriteProtection1;

    /*!< SYSCFG SRAM2 key register,                         Address offset: 0x24

    The following steps are required to unlock the write protection of the SRAM2ER bit in the SYSCFG_CFGR2 register.
        1. Write "0xCA” into Key[7:0]
        2. Write "0x53” into Key[7:0]
        Writing a wrong key reactivates the write protection.
    */

    mmio::bitmask_type<mmio::field_size_type::u32, 8ul, SYSCFG_SKR_KEY_Pos>              SramWriteProtectionKey;

    /*!< SYSCFG SRAM2 write protection register 2,          Address offset: 0x28      */

    mmio::register_type<mmio::field_size_type::u32>                            SramWriteProtection2;
};

static_assert(sizeof(SystemConfigurationLayout) == sizeof(SYSCFG_TypeDef));

/* Page 480 */
template <std::uint32_t kLine>
struct ExtendedLineLayout
{
    /*!< EXTI Interrupt mask register 1,             Address offset: 0x00 */

    /* IMx: Interrupt Mask on line x:
        Bit 0: Interrupt request from Line x is masked.
        Bit 1: Interrupt request from Line x is not masked.
    */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           InterruptMask1;

    /*!< EXTI Event mask register 1,                 Address offset: 0x04 */

    /* EMx: Event Mask on line x:
        Bit 0: Event request from line x is masked.
        Bit 1: Event request from line x is not masked.
    */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           EventMask1;

    /*!< EXTI Rising trigger selection register 1,   Address offset: 0x08 */

    /* RTx: Rising trigger event configuration bit of line x:
        Bit 0: Rising trigger disabled (for Event and Interrupt) for input line.
        Bit 1: Rising trigger enabled (for Event and Interrupt) for input line.
    */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           RisingTriggerSelection1;

    /*!< EXTI Falling trigger selection register 1,  Address offset: 0x0C */

    /* FTx: Falling trigger event configuration bit of line x:
        Bit 0: Falling trigger disabled (for Event and Interrupt) for input line
        Bit 1: Falling trigger enabled (for Event and Interrupt) for input line
    */
    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           FallingTriggerSelection1;

    /*!< EXTI Software interrupt event register 1,   Address offset: 0x10 */

    /* Software interrupt on line x:
        If the interrupt is enabled on this line in the EXTI_IMR, writing a '1' to this bit when it is
        at '0' sets the corresponding pending bit in EXTI_PR resulting in an interrupt request generation.

        This bit is cleared by clearing the corresponding bit in the EXTI_PR register (by writing a ‘1’ into the bit).
    */
    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           SoftwareInterruptEvent1;

    /*!< EXTI Pending register 1,                    Address offset: 0x14 */

    /* Pending interrupt flag on line x:
        Bit 0: No trigger request occurred
        Bit 1: Selected trigger request occurred
        This bit is set when the selected edge event arrives on the interrupt line. This bit is cleared by writing a ‘1’ to the bit.
    */
    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           PendingInterruptFlag1;

    private:

    /*!< Reserved, 0x18                                                   */

    mmio::register_type<mmio::field_size_type::u32>                                          Reserved1;

    /*!< Reserved, 0x1C                                                   */

    mmio::register_type<mmio::field_size_type::u32>                                          Reserved2;

    public:

    /*!< EXTI Interrupt mask register 2,             Address offset: 0x20 */

    /* IMx: Interrupt Mask on line x:
        Bit 0: Interrupt request from Line x is masked.
        Bit 1: Interrupt request from Line x is not masked.
    */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           InterruptMask2;

    /*!< EXTI Event mask register 2,                 Address offset: 0x24 */

    /* EMx: Event Mask on line x:
        Bit 0: Event request from line x is masked.
        Bit 1: Event request from line x is not masked.
    */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           EventMask2;

    /*!< EXTI Rising trigger selection register 2,   Address offset: 0x28 */

    /* RTx: Rising trigger event configuration bit of line x:
        Bit 0: Rising trigger disabled (for Event and Interrupt) for input line.
        Bit 1: Rising trigger enabled (for Event and Interrupt) for input line.
    */
    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           RisingTriggerSelection2;

    /*!< EXTI Falling trigger selection register 2,  Address offset: 0x2C */

     /* FTx: Falling trigger event configuration bit of line x:
        Bit 0: Falling trigger disabled (for Event and Interrupt) for input line
        Bit 1: Falling trigger enabled (for Event and Interrupt) for input line
    */
    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           FallingTriggerSelection2;

    /*!< EXTI Software interrupt event register 2,   Address offset: 0x30 */

    /* Software interrupt on line x:
        If the interrupt is enabled on this line in the EXTI_IMR, writing a '1' to this bit when it is
        at '0' sets the corresponding pending bit in EXTI_PR resulting in an interrupt request generation.

        This bit is cleared by clearing the corresponding bit in the EXTI_PR register (by writing a ‘1’ into the bit).
    */
    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           SoftwareInterruptEvent2;

    /*!< EXTI Pending register 2,                    Address offset: 0x34 */

    /* Pending interrupt flag on line x:
        Bit 0: No trigger request occurred
        Bit 1: Selected trigger request occurred
        This bit is set when the selected edge event arrives on the interrupt line. This bit is cleared by writing a ‘1’ to the bit.
    */
    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, kLine>                           PendingInterruptFlag2;
};

static_assert(sizeof(ExtendedLineLayout<0>) == sizeof(EXTI_TypeDef));

} // namespace sys::hal
