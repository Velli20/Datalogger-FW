#pragma once

namespace sys::dev {

struct joystick_up_type     {};
struct joystick_down_type   {};
struct joystick_left_type   {};
struct joystick_right_type  {};
struct joystick_center_type {};

using event_type = std::variant<
    std::monostate,
    joystick_up_type,
    joystick_down_type,
    joystick_left_type,
    joystick_right_type,
    joystick_center_type
>;

template <device_node k_type>
struct device_type<k_type, typename std::enable_if<is_joystick_device<k_type>::value>::type>
{
    constexpr device_type() = default;

    private:

    static constexpr auto k_configuration = std::get<joystick_config_type>(device_tree::get(k_type));

    template <key_code_type k_keycode, device_node k_node>
    static void interrupt_callback()
    {
        if ( m_callback && hal::peripheral_type<k_node>::read() == true )
        {
            m_callback(input_event::k_key_down, k_keycode);
        }
    }

    void update(const joystick_down_type&)   {}
    void update(const joystick_left_type&)   {}
    void update(const joystick_right_type&)  {}
    void update(const joystick_center_type&) {}
    void update(const joystick_up_type&)     {}

    void update(const event_type& event)
    {
        std::visit(util::overloaded{[](const std::monostate&){}, [](const auto& e) { update(e); } }, event);
    }

    public:

    static auto init(input_callback_type&& callback)
    {
        m_callback = std::move(callback);

        [] <typename T, T... Is> (std::integer_sequence<T, Is...>)
        {
            constexpr auto k_pin_list = k_configuration.pins;

            ([&]
            {
                constexpr auto k_keycode = std::get<0>(k_pin_list[Is]);
                constexpr auto k_pin     = std::get<1>(k_pin_list[Is]);

                hal::peripheral_type<k_pin>::init(interrupt_callback<k_keycode, k_pin>);
            }(), ...);

        }(std::make_integer_sequence<std::size_t, 5 >{});

        return true;
    }

    private:

    static inline input_callback_type m_callback{};
};

} // namespace sys::dev

