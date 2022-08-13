// Includes.

#include <base.hpp>
#include <hal.hpp>

namespace {

// Prevent inlining to avoid persisting any variables on stack

[[gnu::noinline]] void initiliaze_bss_section()
{
    auto* begin_ptr = std::addressof(sys::arch::__bss_start);
    auto* end_ptr   = std::addressof(sys::arch::__bss_end);

    // Clear the .bss segment.

    std::fill(begin_ptr, end_ptr, std::uint8_t{});
}

// Prevent inlining to avoid persisting any variables on stack

[[gnu::noinline]] void initiliaze_data_section()
{
    auto* data_begin_ptr = std::addressof(sys::arch::__data_load_start);
    auto* data_end_ptr   = std::addressof(sys::arch::__data_load_end);
    auto* data_dst_ptr   = std::addressof(sys::arch::__data_start);

    std::copy(data_begin_ptr, data_end_ptr, data_dst_ptr);
}

// Prevent inlining to avoid persisting any variables on stack

[[gnu::noinline]] void initiliaze_init_section()
{
    using namespace sys::arch;

    // WARNING: Be EXTREMELY careful when running code before this function
    // completes. The context before this function violates the C spec
    // (Section 6.7.8, paragraph 10 for example, which requires uninitialized static
    // values to be zero-initialized).

    // Iterate over all the init routines.

    std::for_each(__preinit_array_start, __preinit_array_end, +[](init_function_type p) { p(); });
    std::for_each(__init_array_start,    __init_array_end,    +[](init_function_type p) { p(); });
}

} // namespace

namespace sys::arch {

void startup()
{
    using namespace sys::hal;

    // Disable interrupts.

    arch::disable_interrupts();

    // Delay loop to give debugger chance to reset the CPU.

    for ( std::uint32_t i{}; i < 1000000; ++i )
    {
       arch::no_operation();
    }

    __set_FAULTMASK(0);

    // Disable NVIC interrupts.

    for ( auto& n : NVIC->ICER )
    {
        n = 0xFFFFFFFF;
    }

    // Clear pending NVIC interrupts.

    for ( auto& n : NVIC->ICPR )
    {
        n = 0xFFFFFFFF;
    }

    // Ensure 8-byte alignment of stack pointer on interrupts.

    system_control_block::set_exception_entry_stack_alignment<stack_alignment_type::k_eight_bytes>();

    // Enable fault exception on divide by zero for UDIV/SDIV instructions.

    system_control_block::fault_on_divide_by_zero(true);

    // Enable unaligned memory access.

    system_control_block::fault_on_unaligned_access(false);

    // Enable FPU Access in privileged and user mode.

    system_control_block::set_coprosessor_access_level<coprocessor_access_type::k_full_access>();

    initiliaze_bss_section();
    initiliaze_data_section();

    platform::memory::initialize_free_store(
        reinterpret_cast<std::byte*>(std::addressof(__application_heap_start)),
        reinterpret_cast<std::byte*>(std::addressof(__application_heap_end))
    );

    // Re-enable interrupts.

    arch::enable_interrupts();

    __enable_fault_irq();

    // Call static constructors.

    initiliaze_init_section();

    // Initialize board and its peripherals.

    board::init();

    // Enter main.

    sys::application::start();
}

} // namespace sys::arch
