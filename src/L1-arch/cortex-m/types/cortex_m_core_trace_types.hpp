#pragma once

namespace sys::hal {

enum class data_watch_point_type : std::uint32_t
{
    k_watch_point1 = 0,
    k_watch_point2,
    k_watch_point3,
    k_watch_point4,
};

/* Page C1-722, Table C1-14 DWT  */
enum class data_watch_point_function_type : uint32_t
{
    k_none                     = 0b0000,
    k_instruction_fetch        = 0b0100, // Comparator compares the address for instruction fetches.
    k_memory_read_access       = 0b0101, // Comparator compares the address only for read accesses.
    k_memory_write_access      = 0b0110, // Comparator compares the address only for write accesses.
    k_memory_read_write_access = 0b0111, // Comparator compares the address for read and write accesses.
};

using data_watch_point_callback_type = void (*)(void* user_data);

} // namespace sys::hal
