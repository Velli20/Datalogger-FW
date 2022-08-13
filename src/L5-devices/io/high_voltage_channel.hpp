#pragma once

namespace sys::dev {

template <device_node k_type>
struct device_type<k_type, typename std::enable_if<is_high_voltage_channel_device<k_type>::value>::type>
{
    private:

    static constexpr auto k_device_configuration = std::get<high_voltage_input_config_type>(device_tree::get(k_type));
    static constexpr auto k_peripheral           = k_device_configuration.pin;

    public:

    constexpr device_type() = default;

    [[nodiscard]] static auto read() noexcept -> bool
    {
        return hal::peripheral_type<k_peripheral>::read();
    }

    static auto init() noexcept -> bool
    {
        return hal::peripheral_type<k_peripheral>::init();
    }

    static consteval auto get_readable() noexcept -> readable_type
    {
        return readable_type{read};
    }
};

} // namespace sys::dev
