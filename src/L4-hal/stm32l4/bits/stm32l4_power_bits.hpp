#pragma once

namespace sys::hal {

/* Page 217 */
struct PowerModeLayout
{
    /*!< PWR power control register 1,        Address offset: 0x00      */

    union
    {
        /*  Low-power mode selection:
            These bits select the low-power mode entered when CPU enters the Deepsleep mode.
            Bit 000: Stop 0 mode
            Bit 001: Stop 1 mode
            Bit 010: Stop 2 mode
            Bit 011: Standby mode
            Bit 1xx: Shutdown mode

            Note: If LPR bit is set, Stop 2 mode cannot be selected and Stop 1 mode shall be entered instead of Stop 2.
            In Standby mode, SRAM2 can be preserved or not, depending on RRS bit configuration in PWR_CR3.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  3ul, PWR_CR1_LPMS_Pos, power_mode_type>           LowPowerSelection;

        /*  Disable backup domain write protection:
            In reset state, the RTC and backup registers are protected against parasitic write access. This bit must be set to enable write access to these registers.
            Bit 0: Access to RTC and Backup registers disabled
            Bit 1: Access to RTC and Backup registers enabled
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, PWR_CR1_DBP_Pos>            BackupDomainWriteProtection;

        /*  Voltage scaling range selection:
            Bit 00: Cannot be written (forbidden by hardware)
            Bit 01: Range 1
            Bit 10: Range 2
            Bit 11: Cannot be written (forbidden by hardware)
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, PWR_CR1_VOS_Pos>            RegulatorVoltageScalingSelection;

        /*  Low-power run:
            When this bit is set, the regulator is switched from main mode (MR) to low-power mode (LPR).

            Note: Stop 2 mode cannot be entered when LPR bit is set. Stop 1 is entered instead.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, PWR_CR1_LPR_Pos>            RegulatorLowPowerMode;

    } Control1;

    /*!< PWR power control register 2,        Address offset: 0x04      */

    union
    {
        /*  VDDUSB USB supply valid:
            This bit is used to validate the VDDUSB supply for electrical and logical isolation purpose.
            Setting this bit is mandatory to use the USB OTG_FS peripheral. If VDDUSB is not always present in the application,
            the PVM can be used to determine whether this supply is ready or not.

            Bit 0: VDDUSB is not present. Logical and electrical isolation is applied to ignore this supply.
            Bit 1: VDDUSB is valid.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, PWR_CR2_USV_Pos>            UsbSupplyValid;
    } Control2;

    /*!< PWR power control register 3,        Address offset: 0x08      */

    mmio::register_type<mmio::field_size_type::u32> Control3;

    /*!< PWR power control register 4,        Address offset: 0x0C      */

    mmio::register_type<mmio::field_size_type::u32> Control4;

    /*!< PWR power status register 1,         Address offset: 0x10      */

    mmio::register_type<mmio::field_size_type::u32> SR1;

    /*!< PWR power status register 2,         Address offset: 0x14      */

    mmio::register_type<mmio::field_size_type::u32> SR2;

    /*!< PWR power status reset register,     Address offset: 0x18      */

    mmio::register_type<mmio::field_size_type::u32> SCR;

    /*!< Reserved,                            Address offset: 0x1C      */

    mmio::register_type<mmio::field_size_type::u32> Reserved0;

    /*!< Pull_up control register of portA,   Address offset: 0x20      */

    mmio::register_type<mmio::field_size_type::u32> PUCRA;

    /*!< Pull_Down control register of portA, Address offset: 0x24      */

    mmio::register_type<mmio::field_size_type::u32> PDCRA;

    /*!< Pull_up control register of portB,   Address offset: 0x28      */

    mmio::register_type<mmio::field_size_type::u32> PUCRB;

    /*!< Pull_Down control register of portB, Address offset: 0x2C      */

    mmio::register_type<mmio::field_size_type::u32> PDCRB;

    /*!< Pull_up control register of portC,   Address offset: 0x30      */

    mmio::register_type<mmio::field_size_type::u32> PUCRC;

    /*!< Pull_Down control register of portC, Address offset: 0x34      */

    mmio::register_type<mmio::field_size_type::u32> PDCRC;

    /*!< Pull_up control register of portD,   Address offset: 0x38      */

    mmio::register_type<mmio::field_size_type::u32> PUCRD;

    /*!< Pull_Down control register of portD, Address offset: 0x3C      */

    mmio::register_type<mmio::field_size_type::u32> PDCRD;

    /*!< Pull_up control register of portE,   Address offset: 0x40      */

    mmio::register_type<mmio::field_size_type::u32> PUCRE;

    /*!< Pull_Down control register of portE, Address offset: 0x44      */

    mmio::register_type<mmio::field_size_type::u32> PDCRE;

    /*!< Pull_up control register of portF,   Address offset: 0x48      */

    mmio::register_type<mmio::field_size_type::u32> PUCRF;

    /*!< Pull_Down control register of portF, Address offset: 0x4C      */

    mmio::register_type<mmio::field_size_type::u32> PDCRF;

    /*!< Pull_up control register of portG,   Address offset: 0x50      */

    mmio::register_type<mmio::field_size_type::u32> PUCRG;

    /*!< Pull_Down control register of portG, Address offset: 0x54      */

    mmio::register_type<mmio::field_size_type::u32> PDCRG;

    /*!< Pull_up control register of portH,   Address offset: 0x58      */

    mmio::register_type<mmio::field_size_type::u32> PUCRH;

    /*!< Pull_Down control register of portH, Address offset: 0x5C      */

    mmio::register_type<mmio::field_size_type::u32> PDCRH;

    /*!< Pull_up control register of portI,   Address offset: 0x60      */

    mmio::register_type<mmio::field_size_type::u32> PUCRI;

    /*!< Pull_Down control register of portI, Address offset: 0x64      */

    mmio::register_type<mmio::field_size_type::u32> PDCRI;

    /*!< Reserved,                            Address offset: 0x68-0x7C */

    mmio::register_type<mmio::field_size_type::u32> Reserved1[6];

    /*!< PWR power control register 5,        Address offset: 0x80      */

    mmio::register_type<mmio::field_size_type::u32> CR5;
};

static_assert(sizeof(PowerModeLayout) == 132);

} // namespace sys::hal
