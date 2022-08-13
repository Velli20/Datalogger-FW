#pragma once

namespace sys::hal {

enum class stack_alignment_type : std::uint32_t
{
    k_four_bytes  = 0b0u,
    k_eight_bytes = 0b1u
};

enum class coprocessor_access_type : std::uint32_t
{
    k_access_denied          = 0b00u,
    k_privileged_access_only = 0b01u,
    k_full_access            = 0b11u
};

} // namespace sys::hal
