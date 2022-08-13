#pragma once

namespace sys::hal {

template <device_node k_node> requires (detail::instance_of_gpio_node<k_node>::value)
struct peripheral_type<k_node> final
{
    private:

    static constexpr auto k_base_address = register_address_of_node<k_node>::value;
    static constexpr auto k_pin          = unit_sub_address_of_node<k_node>::value;

    static_assert(k_pin >= 0 && k_pin <= 15, "Invalid pin number");

    [[nodiscard]] static constexpr auto& get() noexcept
    {
        return mmio::place_at<GpioLayout<k_pin>>(k_base_address);
    }

    public:

    static constexpr auto init() noexcept -> bool
    {
        constexpr auto k_configuration = hal::gpio_pin_of(k_node).config;

        // Enable selected GPIO peripheral clock.

        hal::peripheral_type<"/chip/rcc">::enable<k_node>();

        get().Alternate().Write(k_configuration.alternate);
        get().Mode.Write(k_configuration.mode);
        get().OutputSpeed.Write(k_configuration.speed);
        get().PortPull.Write(k_configuration.pull);

        return true;
    }

    static constexpr auto init(hal::concepts::exti_handler auto callback) noexcept -> bool
    {
        if ( init() == false )
        {
            return false;
        }

        constexpr auto k_configuration = hal::gpio_pin_of(k_node).config;

        static_assert(k_configuration.mode == gpio_mode_type::k_interrupt_falling ||
                      k_configuration.mode == gpio_mode_type::k_interrupt_rising  ||
                      k_configuration.mode == gpio_mode_type::k_interrupt_rising_falling, "Requested pin does not have interrupt configuration.");

        return hal::peripheral_type<"/chip/exti">::enable_external_interrupt<k_node>(callback);
    }

    static constexpr auto set()
    {
        get().BitSetReset.Set();
    }

    static constexpr auto reset()
    {
        get().BitReset.Set();
    }

    static constexpr auto toggle()
    {
        get().OutputData.Toggle();
    }

    [[nodiscard]] static constexpr auto read() noexcept -> bool
    {
        return get().InputData.Read() == 1ul;
    }

    [[nodiscard]] static constexpr auto is_set() noexcept -> bool
    {
        return get().OutputData.Read() == 1ul;
    }

    static constexpr auto lock()
    {
        get().ConfigLock.Set();
    }
};

} // namespace sys::hal

