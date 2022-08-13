#pragma once

namespace sys::hal {

template <>
struct peripheral_type<"/chip/power"> final
{
    private:

    static constexpr PowerModeLayout& get() noexcept
    {
        constexpr auto k_base_address = register_address_of_node<"/chip/power">::value;

        return mmio::place_at<PowerModeLayout>(k_base_address);
    }

    public:

    static constexpr void disable_backup_domain_protection(const bool disable = true) noexcept
    {
        get().Control1.BackupDomainWriteProtection.Write(disable ? 1ul : 0ul);
    };

    template <power_mode_type k_mode>
    static constexpr auto set_low_power_mode() noexcept
    {
        if constexpr ( k_mode == power_mode_type::k_stop_two )
        {
            // Cannot enter STOP2-mode if Low Power Run -mode is enabled.

            if ( get().Control1.RegulatorLowPowerMode.Read() != 0ul )
                return false;
        }

        get().Control1.LowPowerSelection.Write(k_mode);

        return true;
    };

    static constexpr auto enable_low_power_mode(bool enable = true) noexcept
    {
        get().Control1.RegulatorLowPowerMode.Write(enable ? 1ul : 0ul);
    };

    static constexpr auto set_usb_power_supply_valid(bool valid) noexcept
    {
        get().Control2.UsbSupplyValid.Write(valid ? 1ul : 0ul);
    };
};

} // namespace sys::hal
