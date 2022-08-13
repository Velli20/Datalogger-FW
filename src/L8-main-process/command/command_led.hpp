#pragma once

namespace sys {

template <>
struct shell::command_type<"led-on"> final
{
    static auto on_command_match(const shell::arguments& argv, log::formatting_stream<shell::input_line>& os)
    {
        using namespace std::string_view_literals;

        os << "Led-on"sv << log::endl;

        for ( auto sv : argv )
        {
            os << sv.data() << log::endl;
        }

        os::scheduler::signal<"Sensor">(os::static_task<"Sensor">::k_signal_userled_on);
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Set user LED on."sv;
    }
};

template <>
struct shell::command_type<"led-off"> final
{
    static auto on_command_match()
    {
        os::scheduler::signal<"Sensor">(os::static_task<"Sensor">::k_signal_userled_off);
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Set user LED off."sv;
    }
};

template <>
struct shell::command_type<"led-toggle"> final
{
    static auto on_command_match()
    {
        os::scheduler::signal<"Sensor">(os::static_task<"Sensor">::k_signal_userled_toggle);
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Toggle user LED."sv;
    }
};

} // namespace sys
