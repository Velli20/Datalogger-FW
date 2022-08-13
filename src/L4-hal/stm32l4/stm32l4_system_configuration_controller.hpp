#pragma once

namespace sys::hal {

template <>
struct peripheral_type<"/chip/system-configuration">
{
    private:

    [[nodiscard]] static constexpr auto& get() noexcept
    {
        constexpr auto k_base_address = register_address_of_node<"/chip/system-configuration">::value;

        return mmio::place_at<SystemConfigurationLayout>(k_base_address);
    }

    public:

    static constexpr auto init()
    {
        hal::peripheral_type<"/chip/rcc">::enable<"/chip/system-configuration">();

        return true;
    }

    template <device_node k_node> requires (detail::instance_of_gpio_node<k_node>::value)
    static constexpr auto set_external_line_interrupt_source() noexcept
    {
        constexpr auto k_pin = unit_sub_address_of_node<k_node>::value;

        constexpr std::uint32_t k_value = []()
        {
            if ( instance_of_node<k_node, "/chip/gpio-a@0">::value ) { return 0b0000; }
            if ( instance_of_node<k_node, "/chip/gpio-b@0">::value ) { return 0b0001; }
            if ( instance_of_node<k_node, "/chip/gpio-c@0">::value ) { return 0b0010; }
            if ( instance_of_node<k_node, "/chip/gpio-d@0">::value ) { return 0b0011; }
            if ( instance_of_node<k_node, "/chip/gpio-e@0">::value ) { return 0b0100; }
            if ( instance_of_node<k_node, "/chip/gpio-f@0">::value ) { return 0b0101; }
            if ( instance_of_node<k_node, "/chip/gpio-g@0">::value ) { return 0b0110; }
            if ( instance_of_node<k_node, "/chip/gpio-h@0">::value ) { return 0b0111; }
            if ( instance_of_node<k_node, "/chip/gpio-i@0">::value ) { return 0b1000; }
        }();

        get().ExternalInterruptConfiguration<k_pin>().Write(k_value);
    }
};

} // namespace sys::hal
