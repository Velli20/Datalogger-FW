#pragma once

namespace sys::log {

namespace detail {

[[nodiscard]] constexpr auto weekday(const std::tm& t, bool full)
{
    using namespace std::string_view_literals;

    constexpr std::string_view k_week_days_short[] =
    {
        "Mon"sv, "Tue"sv, "Wed"sv, "Thu"sv, "Fri"sv, "Sat"sv, "Sun"sv
    };

    constexpr std::string_view k_week_days_long[] =
    {
        "Sunday"sv,
        "Monday"sv,
        "Tuesday"sv,
        "Wednesday"sv,
        "Thursday"sv,
        "Friday"sv,
        "Saturday"sv,
    };

    if ( t.tm_wday >= 0 && t.tm_wday < 7 )
    {
        return full ? k_week_days_long[t.tm_wday] : k_week_days_short[t.tm_wday];
    }

    return "ERR"sv;
}

[[nodiscard]] constexpr auto month(const std::tm& t, bool full)
{
    using namespace std::string_view_literals;

    constexpr std::string_view k_months_short[] =
    {
        "Jan"sv, "Feb"sv, "Mar"sv, "Apr"sv, "May"sv, "Jun"sv,
        "Jul"sv, "Aug"sv, "Sep"sv, "Oct"sv, "Nov"sv, "Dec"sv
    };

    constexpr std::string_view k_months_long[] =
    {
        "January"sv,
        "February"sv,
        "March"sv,
        "April"sv,
        "May"sv,
        "June"sv,
        "July"sv,
        "August"sv,
        "September"sv,
        "October"sv,
        "November"sv,
        "December"sv,
    };

    if ( t.tm_mon >= 0 && t.tm_mon < 12 )
    {
        return full ? k_months_long[t.tm_mon] : k_months_short[t.tm_mon];
    }

    return "ERR"sv;
}

} // namespace detail

template <class T>
constexpr auto operator<<(
    log::formatting_stream<T>& os,
    const std::tm&             t
) noexcept -> log::formatting_stream<T>&
{
    os << detail::weekday(t, false) << ' ';
    os << detail::month(t, false)   << ' ';

    // Thu Jan 01 00:00:00 1970OK

    if ( t.tm_mday < 10 )
    {
        os << '0';
    }

    os << t.tm_mday << ' ';

    if ( t.tm_hour < 10 )
    {
        os << '0';
    }

    os << t.tm_hour << ':';

    if ( t.tm_min < 10 )
    {
        os << '0';
    }

    os << t.tm_min << ':';

    if ( t.tm_sec < 10 )
    {
        os << '0';
    }

    os << t.tm_sec << ' ';
    os << t.tm_year + 1900;

    return os;
}

} // namespace sys::log
