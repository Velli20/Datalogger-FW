#pragma once

namespace sys::hal {

template <>
struct peripheral_type<"/chip/rtc"> final
{
    private:

    static constexpr auto         k_configuration = std::get<rtc_phy_config_type>(hal::peripheral_of("/chip/rtc").config);
    static constinit auto inline  m_initialized   = std::atomic<bool>{};

    [[nodiscard]] static constexpr auto get() noexcept -> RtcLayout&
    {
        constexpr auto k_rtc_base = register_address_of_node<"/chip/rtc">::value;

        return mmio::place_at<RtcLayout>(k_rtc_base);
    }

    [[nodiscard]] static constexpr auto is_synchronized() noexcept
    {
        // Checks if registers are synchronized.

        return get().Initialization.RegisterSyncronizedFlag.Read() == 0b1ul ? true : false;
    }

    static constexpr auto invalidate() noexcept
    {
        // Clear registers synchronized flag.

        get().Initialization.RegisterSyncronizedFlag.Clear();
    }

    static constexpr void enable_write_protection(const bool enable = true) noexcept
    {
        // Write protection: Reading this byte always returns 0x00.

        if ( enable )
        {
            get().WriteProtection.Key.Write(0xFFul);
        }

        else
        {
            get().WriteProtection.Key.Write(0xCAul);
            get().WriteProtection.Key.Write(0x53ul);
        }
    }

    struct initialization_mode
    {
        [[nodiscard]] static constexpr auto is_enabled() noexcept
        {
            return get().Initialization.InternalTimeStampFlag.Read() == 0b1ul;
        }

        static constexpr auto disable() noexcept
        {
            // Bit 0: Free running mode.
            // Bit 1: Initialization mode used to program time and date register, and prescaler register.
            //        Counters are stopped and start counting from the new value when INIT is reset.

            get().Initialization.Enable.Clear();
        }

        static constexpr auto enable() noexcept
        {
            // Check if the Initialization mode is set.

            if ( is_enabled() )
                return true;

            // Enable the Initialization mode.

            get().Initialization.Enable.Set();

            using namespace ::std::chrono_literals;

            // Wait until RTC is in intialization mode or return false if timeout is reached.

            return util::busy_wait(2000ms, is_enabled);
        }
    };

    [[nodiscard]] static constexpr bool is_shadow_registers_bypass_enabled() noexcept
    {
        // Bit 0: Calendar values are taken from the shadow registers, which are updated once every two RTCCLK cycles.
        // Bit 1: Calendar values are taken directly from the calendar counters.

        return get().Control.BypassShadowRegisters.Read() == 0b0ul;
    }

    struct rtc_wp_lock_type
    {
        void lock()   { enable_write_protection(false); }
        void unlock() { enable_write_protection(true);  }
    };

    public:

    [[maybe_unused]] static auto init() noexcept -> bool
    {
        if ( auto was_initialized = m_initialized.exchange(true); was_initialized == true )
        {
            return true;
        }

        // Enable RTC.

        hal::peripheral_type<"/chip/rcc">::enable<"/chip/rtc">();

        // Enable backup register.

        hal::peripheral_type<"/chip/rcc">::enable<"/chip/power">();

        // Disable Back-up domain R/W-protection.

        hal::peripheral_type<"/chip/power">::disable_backup_domain_protection(true);

        // RAII style R/W-protection lock.

        rtc_wp_lock_type lk{};
        std::lock_guard<rtc_wp_lock_type> lock(lk);

        // Enter initialization mode.

        if ( initialization_mode::enable() == false )
        {
            return m_initialized.exchange(false) == false;
        }

        // Reset RTC peripheral control register.

        get().Control.Word.Clear();

        // Set output polarity ...

        get().Control.OutputPolarity.Write(k_configuration.polarity);

        // ... output selection ...

        get().Control.OutputSelection.Write(k_configuration.output);

        // ... hour format ...

        get().Control.HourFormat.Write(k_configuration.format);

        // ... synchronous divider ...

        get().Prescaler.Synchronous.Write(k_configuration.synchronous_divider);

        // ... asynchronous divider ...

        get().Prescaler.Asynchronous.Write(k_configuration.asynchronous_divider);

        // ... finally exit initialization mode.

        initialization_mode::disable();

        return true;
    }

    [[maybe_unused]] static bool get_time(std::tm& tm)
    {
        constexpr auto bcd_to_int = [](std::uint8_t bcd) -> std::uint8_t
        {
            return (bcd & 0x0F) + (bcd >> 4) * 10;
        };

        const auto time_value = get().Time.View();
        const auto date_value = get().Date.View();

        tm.tm_sec  = bcd_to_int(time_value.Read<RtcLayout::Time::Seconds>());
        tm.tm_min  = bcd_to_int(time_value.Read<RtcLayout::Time::Minutes>());
        tm.tm_hour = bcd_to_int(time_value.Read<RtcLayout::Time::Hours>());

        tm.tm_wday = bcd_to_int(date_value.Read<RtcLayout::Date::DayOfYear>());
        tm.tm_mday = bcd_to_int(date_value.Read<RtcLayout::Date::Day>());
        tm.tm_mon  = bcd_to_int(date_value.Read<RtcLayout::Date::Month>());
        tm.tm_year = bcd_to_int(date_value.Read<RtcLayout::Date::Year>()) + 100;

        return true;
    }

    [[maybe_unused]] static auto set_time(const std::tm& tm) noexcept
    {
        constexpr auto int_to_bcd = [](std::uint8_t integer) -> std::uint32_t
        {
            return static_cast<std::uint32_t>((integer % 10) + ((integer / 10) << 4));
        };

        rtc_wp_lock_type lk{};
        std::lock_guard<rtc_wp_lock_type> lock(lk);

        // Enter initialization mode.

        if ( initialization_mode::enable() == false )
        {
            return false;
        }

        get().Time
             .Edit()
             .Write<RtcLayout::Time::Seconds>(int_to_bcd(tm.tm_sec))
             .Write<RtcLayout::Time::Minutes>(int_to_bcd(tm.tm_min))
             .Write<RtcLayout::Time::Hours>  (int_to_bcd(tm.tm_hour))
             .Save();

        get().Date
             .Edit()
             .Write<RtcLayout::Date::Day>       (int_to_bcd(tm.tm_mday))
             .Write<RtcLayout::Date::Month>     (int_to_bcd(tm.tm_mon))
             .Write<RtcLayout::Date::Year>      (int_to_bcd(tm.tm_year - 100 - 1900))
             .Write<RtcLayout::Date::DayOfYear> (int_to_bcd(tm.tm_wday))
             .Save();

        // Exit Initialization mode.

        initialization_mode::disable();

        // Return immediatly if no need for synchronization.

        if ( is_shadow_registers_bypass_enabled() == false )
        {
            return true;
        }

        // Clear Registers synchronization flag.

        invalidate();

        using namespace ::std::chrono_literals;

        if ( is_synchronized() && util::busy_wait(2000ms, is_synchronized) == false )
        {
            return false;
        }

        return util::busy_wait(2000ms, is_synchronized);
    }
};

} // namespace sys::hal
