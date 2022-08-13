#pragma once

namespace sys::hal {

enum class rtc_hour_format_type : std::uint32_t
{
    k_24h = 0b0,
    k_12h = 0b1,
};

enum class rtc_output_type : std::uint32_t
{
    k_none   = 0b00,
    k_alarm1 = 0b01,
    k_alarm2 = 0b10,
    k_wakeup = 0b11,
};

enum class rtc_output_polarity_type : std::uint32_t
{
    k_pin_high = 0b0,
    k_pin_low  = 0b1,
};

struct rtc_phy_config_type
{
    rtc_output_polarity_type polarity;
    rtc_output_type          output;
    rtc_hour_format_type     format;
    std::uint32_t            synchronous_divider;
    std::uint32_t            asynchronous_divider;
};

} // namespace sys::hal

namespace sys {

template <>
struct is_bitmask_enum <hal::rtc_hour_format_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::rtc_output_type> : std::true_type {};

template <>
struct is_bitmask_enum <hal::rtc_output_polarity_type> : std::true_type {};

} // namespace sys
