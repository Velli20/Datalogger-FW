#pragma once

namespace sys::hal {

enum class power_mode_type : std::uint32_t
{
    k_standby   = 0b000,
    k_shut_down = 0b001,
    k_stop_one  = 0b010,
    k_stop_two  = 0b011,
    k_stop_zero = 0b100
};

} // namespace sys::hal

namespace sys {

template <>
struct is_bitmask_enum <hal::power_mode_type> : std::true_type {};

} // namespace sys
