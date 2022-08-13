#pragma once

namespace sys::hal {

template <>
struct peripheral_type<"/chip/usb-otg"> final
{
    static constexpr auto k_usb_config = std::get<usb_phy_config_type>(hal::peripheral_of("/chip/usb-otg").config);
    static constexpr auto k_dm_pin     = k_usb_config.dm_pin;
    static constexpr auto k_dp_pin     = k_usb_config.dp_pin;

    inline static /* constexpr */ auto init() noexcept
    {
        if ( auto was_initialized = m_initialized.exchange(true); was_initialized == true )
        {
            return true;
        }

        // Enable USB-OTG peripheral clock.

        hal::peripheral_type<"/chip/rcc">::enable<"/chip/usb-otg">();

        const bool pwr_clock_enabled = hal::peripheral_type<"/chip/rcc">::is_enabled<"/chip/power">();

        // Set USB power supply to valid.

        if ( pwr_clock_enabled == false )
        {
            hal::peripheral_type<"/chip/rcc">::enable<"/chip/power">();
        }

        hal::peripheral_type<"/chip/power">::set_usb_power_supply_valid(true);

        if ( pwr_clock_enabled == false )
        {
            hal::peripheral_type<"/chip/rcc">::disable<"/chip/power">();
        }

        // Configure DM and DP pins.

        hal::peripheral_type<k_dm_pin>::init();
        hal::peripheral_type<k_dp_pin>::init();

        // Enable interrupts.

        arch::nvic::interrupt_enable<"/chip/usb-otg">();

        return true;
    }

    template <arch::arm::vendor_irqn_type k_type>
    static constexpr auto interrupt_handler() noexcept
    {
        extern PCD_HandleTypeDef m_pcd_handle;

        HAL_PCD_IRQHandler(std::addressof(m_pcd_handle));
    }

    private:

    static inline constinit std::atomic<bool> m_initialized{};
    static inline platform::binary_semaphore  m_data_available_semaphore{};
    static inline platform::mutex             m_read_lock;
    static inline platform::mutex             m_write_lock;
};

} // namespace sys::hal
