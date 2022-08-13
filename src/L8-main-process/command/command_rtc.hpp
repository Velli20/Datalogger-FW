#pragma once

namespace sys {

template <>
struct shell::command_type<"rtc-get-time"> final
{
    static auto on_command_match(const shell::arguments& args)
    {
        std::tm tim;

        using namespace sys::hal;
        using namespace std::string_view_literals;

        // Get RTC timestamp.

        if ( hal::peripheral_type<"/chip/rtc">::get_time(tim) == true )
        {
            log::info() << "RTC time:"sv << tim << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Print RTC time"sv;
    }
};

template <>
struct shell::command_type<"rtc-set-time"> final
{
    static auto on_command_match(const shell::arguments& args)
    {
        if ( args.size() < 3 )
            return;

        using namespace sys::hal;

        std::tm tim;

        // Get RTC timestamp.

        if ( hal::peripheral_type<"/chip/rtc">::get_time(tim) == true )
        {
            // Parse values.

            tim.tm_hour = std::atoi(args[0].data());
            tim.tm_min  = std::atoi(args[1].data());
            tim.tm_sec  = std::atoi(args[2].data());

            // Set time.

            hal::peripheral_type<"/chip/rtc">::set_time(tim);
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Set RTC time. <HH> <MM> <SS>"sv;
    }
};

template <>
struct shell::command_type<"rtc-set-date"> final
{
    static auto on_command_match(const shell::arguments& args)
    {
        if ( args.size() < 3 )
        {
            return;
        }

        using namespace sys::hal;

        std::tm tim;

        // Get RTC timestamp.

        if ( hal::peripheral_type<"/chip/rtc">::get_time(tim) == true )
        {
            // Parse values.

            tim.tm_mday = std::atoi(args[0].data());
            tim.tm_mon  = std::atoi(args[1].data());
            tim.tm_year = std::atoi(args[2].data());

            // Set time.

            hal::peripheral_type<"/chip/rtc">::set_time(tim);
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Set RTC date. <DD> <MM> <YYYY>"sv;
    }
};

} // namespace sys