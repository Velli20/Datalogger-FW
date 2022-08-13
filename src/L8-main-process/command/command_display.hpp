#pragma once

namespace sys {

template <>
struct shell::command_type<"display-on"> final
{
    static auto on_command_match()
    {
        os::scheduler::signal<"Display">(os::static_task<"Display">::k_signal_display_on);
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Set display on."sv;
    }
};

template <>
struct shell::command_type<"display-off"> final
{
    static auto on_command_match()
    {
        os::scheduler::signal<"Display">(os::static_task<"Display">::k_signal_display_off);
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Set display off."sv;
    }
};

template <>
struct shell::command_type<"display-invert"> final
{
    static auto on_command_match()
    {
        os::scheduler::signal<"Display">(os::static_task<"Display">::k_signal_display_invert);
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Invert display colors."sv;
    }
};

} // namespace sys
