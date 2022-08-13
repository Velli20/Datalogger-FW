#pragma once

namespace sys::hal {

enum class external_line_port_type : std::uint32_t
{
    k_port_a = 0b0000,
    k_port_b = 0b0001,
    k_port_c = 0b0010,
    k_port_d = 0b0011,
    k_port_e = 0b0100,
    k_port_f = 0b0101,
    k_port_g = 0b0110,
    k_port_h = 0b0111,
    k_port_i = 0b1000,
};

using external_line_callback = void (*)();

} // namespace sys::hal

namespace sys {

template <>
struct is_bitmask_enum <hal::external_line_port_type> : std::true_type {};

} // namespace sys
