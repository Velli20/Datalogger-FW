#pragma once

namespace sys::log {

enum class format_flag : std::uint32_t
{
    kClear        = 0b1000'00000'00000,
    k_hex_format          = 0b0000'00000'00001,
    k_bin_format          = 0b0000'00000'00010,
    k_decimal_format          = 0b0000'00000'00100,
    kShowBase     = 0b0000'00000'01000,
    kNoShowBase   = kShowBase | kClear,
    kUpperCase    = 0b0000'00000'10000,
    kNoUpperCase  = kUpperCase | kClear,
    kSetWidth     = 0b0001'00000'00000,
    kNoSetWidth   = kSetWidth | kClear,

    kNone         = 0
};

} // namespace sys::log

namespace sys {

template <>
struct is_bitmask_enum <log::format_flag> : std::true_type {};

} // namespace sys
