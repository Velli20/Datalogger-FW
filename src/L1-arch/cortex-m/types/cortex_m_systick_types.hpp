#pragma once

#include <cstddef>
#include <cstdint>

namespace sys::hal {

enum class sys_tick_source_type : std::uint32_t
{
    k_reference_clock = 0u,
    k_processor_clock = 1u,
};

struct sys_tick_config_type
{
    sys_tick_source_type source;
    std::uint32_t        tick_frequency;
};

} // namespace sys::hal
