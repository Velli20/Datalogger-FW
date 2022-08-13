#pragma once

namespace sys::dev {

template <device_node k_type>
struct device_type<k_type, typename std::enable_if<is_led_device<k_type>::value>::type>
{
    private:

    static constexpr auto k_device_configuration = std::get<led_config_type>(device_tree::get(k_type));
    static constexpr auto k_peripheral           = k_device_configuration.pin;

    public:

    static auto on()
    {
        hal::peripheral_type<k_peripheral>::set();
    }

    static auto off()
    {
        hal::peripheral_type<k_peripheral>::reset();
    }

    static auto toggle()
    {
        hal::peripheral_type<k_peripheral>::toggle();
    }

    static auto init() noexcept -> bool
    {
        return hal::peripheral_type<k_peripheral>::init();
    }
};

} // namespace sys::dev

