#pragma once

namespace sys::arch {

using init_function_type = void(*)(void);

extern "C" {

extern std::uint8_t __bss_start;         // Start address for the .bss section.
extern std::uint8_t __bss_end;           // End address for the .bss section.

extern std::uint8_t __data_start;        // Start address for the .data section.
extern std::uint8_t __data_load_start;   // Load address for the .data section.
extern std::uint8_t __data_load_end;     // Load address for the .data section.

extern std::uint8_t __application_heap_start;  // Start address for the application heap section.
extern std::uint8_t __application_heap_end;    // End address for the application heap section.

extern init_function_type __preinit_array_start[0];
extern init_function_type __preinit_array_end[0];
extern init_function_type __init_array_start[0];
extern init_function_type __init_array_end[0];

extern void MainStackTop(void);
extern void MainStackBottom(void);

}

} // namespace sys::arch
