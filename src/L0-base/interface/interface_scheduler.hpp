#pragma once

namespace sys::os {

enum class service_call_code_type : std::uint32_t
{
    k_kernel_start = 0,
    k_allocate_memory,
    k_free_memory
};

enum class mode_type : std::uint32_t
{
    k_handler = 0,
    k_thread,
};

} // namespace sys::os
