#pragma once

namespace sys::hal {

struct RtcLayout
{
    /*!< RTC time register,                                         Address offset: 0x00      */

    struct Time final : mmio::register_type<mmio::field_size_type::u32>
    {
        using Seconds       = mmio::register_mask_type<mmio::field_size_type::u32,  7ul, RTC_TSTR_SU_Pos>;
        using Minutes       = mmio::register_mask_type<mmio::field_size_type::u32,  7ul, RTC_TSTR_MNU_Pos>;
        using Hours         = mmio::register_mask_type<mmio::field_size_type::u32,  6ul, RTC_TSTR_HU_Pos>;
        using AmPmNotation  = mmio::register_mask_type<mmio::field_size_type::u32,  1ul, RTC_TSTR_PM_Pos>;
    } Time;

    /*!< RTC date register,                                         Address offset: 0x04      */

    struct Date final : mmio::register_type<mmio::field_size_type::u32>
    {
        using Day           = mmio::register_mask_type<mmio::field_size_type::u32,  6ul, RTC_DR_DU_Pos>;
        using Month         = mmio::register_mask_type<mmio::field_size_type::u32,  4ul, RTC_DR_MU_Pos>;
        using DayOfYear     = mmio::register_mask_type<mmio::field_size_type::u32,  3ul, RTC_DR_WDU_Pos>;
        using Year          = mmio::register_mask_type<mmio::field_size_type::u32,  8ul, RTC_DR_YT_Pos>;
    } Date;

    /*!< RTC control register,                                      Address offset: 0x08      */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, RTC_CR_FMT_Pos, rtc_hour_format_type>        HourFormat;
        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, RTC_CR_OSEL_Pos, rtc_output_type>           OutputSelection;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, RTC_CR_POL_Pos, rtc_output_polarity_type>    OutputPolarity;

        // Bit 0: Calendar values are taken from the shadow registers, which are updated once every two RTCCLK cycles.
        // Bit 1: Calendar values are taken directly from the calendar counters.

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, RTC_CR_BYPSHAD_Pos>    BypassShadowRegisters;
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                   Word;
    } Control;

    /*!< RTC initialization and status register,                    Address offset: 0x0C      */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, RTC_ISR_INIT_Pos>      Enable;

        // When this bit is set to 1, the RTC is in initialization state, and the time, date and prescaler registers can be updated.
        // Bit 0: Calendar registers update is not allowed.
        // Bit 1: Calendar registers update is allowed.

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, RTC_ISR_INITF_Pos>     InternalTimeStampFlag;

         // This bit is set by hardware each time the calendar registers are copied into
        // the shadow registers (RTC_SSRx, RTC_TRx and RTC_DRx).
        // This bit is cleared by hardware in initialization mode, while a shift operation is
        // pending (SHPF=1), or when in bypass shadow register mode (BYPSHAD=1).
        // This bit can also be cleared by software. It is cleared either by software or by hardware in initialization mode.

        // Bit 0: Calendar shadow registers not yet synchronized
        // Bit 1: Calendar shadow registers synchronized

        // Bit 0: Free running mode.
        // Bit 1: Initialization mode used to program time and date register, and prescaler register.
        //        Counters are stopped and start counting from the new value when INIT is reset.

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, RTC_ISR_RSF_Pos>       RegisterSyncronizedFlag;
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                   Word;
    } Initialization;

    /*!< RTC prescaler register,                                    Address offset: 0x10      */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 16ul, RTC_PRER_PREDIV_A_Pos> Asynchronous;
        mmio::bitmask_type<mmio::field_size_type::u32, 16ul, RTC_PRER_PREDIV_S_Pos> Synchronous;
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                   Word;
    } Prescaler;

    mmio::register_type<mmio::field_size_type::u32> WakeupTimer;          /*!< RTC wakeup timer register,                                 Address offset: 0x14      */
    mmio::register_type<mmio::field_size_type::u32> Reserved;             /*!< Reserved                                                                             */
    mmio::register_type<mmio::field_size_type::u32> AlarmA;               /*!< RTC alarm A register,                                      Address offset: 0x1C      */
    mmio::register_type<mmio::field_size_type::u32> AlarmB;               /*!< RTC alarm B register,                                      Address offset: 0x20      */

    /*!< RTC write protection register,                             Address offset: 0x24      */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                    Word;
        mmio::bitmask_type<mmio::field_size_type::u32, 8ul,  RTC_WPR_KEY_Pos>        Key;
    } WriteProtection;

    mmio::register_type<mmio::field_size_type::u32> SubSecond;           /*!< RTC sub second register,                                   Address offset: 0x28      */
    mmio::register_type<mmio::field_size_type::u32> ShiftControl;        /*!< RTC shift control register,                                Address offset: 0x2C      */
    mmio::register_type<mmio::field_size_type::u32> TimeStampTime;       /*!< RTC time stamp time register,                              Address offset: 0x30      */
    mmio::register_type<mmio::field_size_type::u32> TimeStampDate;       /*!< RTC time stamp date register,                              Address offset: 0x34      */
    mmio::register_type<mmio::field_size_type::u32> TimeStampSubSecond;  /*!< RTC time-stamp sub second register,                        Address offset: 0x38      */
    mmio::register_type<mmio::field_size_type::u32> Calibration;         /*!< RTC calibration register,                                  Address offset: 0x3C      */
    mmio::register_type<mmio::field_size_type::u32> TemperConfiguration; /*!< RTC tamper configuration register,                         Address offset: 0x40      */
    mmio::register_type<mmio::field_size_type::u32> AlarmSubSecondA;     /*!< RTC alarm A sub second register,                           Address offset: 0x44      */
    mmio::register_type<mmio::field_size_type::u32> AlarmSubSecondB;     /*!< RTC alarm B sub second register,                           Address offset: 0x48      */
    mmio::register_type<mmio::field_size_type::u32> Option;              /*!< RTC option register,                                       Address offset: 0x4C      */
    mmio::register_type<mmio::field_size_type::u32> Backup[32];          /*!< RTC backup register 0-31,                                  Address offset: 0x50-0xCC */
};

static_assert(sizeof(RtcLayout) == 208);

} // namespace sys::hal
