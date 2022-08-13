
#pragma once

namespace sys::arch {

enum class control_mode_type : std::uint32_t
{
    k_unprivileged = 0b001, /* Thread mode has unprivileged access */
    k_privileged   = 0b010, /* Thread mode uses SP_process stack */
    k_fpu_active   = 0b100, /* FP extension is active */
};

[[gnu::always_inline]] static inline void set_control(control_mode_type control)
{
    asm volatile("msr control, %0" : : "r" (control) : "memory");
}

[[gnu::always_inline]] static inline void memory_reorder_barrier()
{
    asm volatile("":::"memory");
}

[[gnu::always_inline]] static inline void instruction_synchronization_barrier()
{
    asm volatile("isb 0xF":::"memory");
}

[[gnu::always_inline]] static inline void data_synchronization_barrier()
{
    asm volatile("dsb 0xF":::"memory");
}

[[gnu::always_inline]] static inline void data_memory_barrier()
{
    asm volatile("dmb 0xF":::"memory");
}

[[gnu::always_inline]] static inline void disable_interrupts()
{
    asm volatile("cpsid i");
}

[[gnu::always_inline]] static inline void enable_interrupts()
{
    asm volatile("cpsie i");
}

template <std::uint32_t k_number> [[gnu::always_inline]] static inline void break_point()
{
    asm volatile("bkpt %[immediate]" ::[immediate] "I" (k_number));
}

[[gnu::always_inline]] static inline void set_stack_address(std::uint32_t address)
{
    asm volatile(R"(
        mov r1, %[stack_top]
        mov sp, r1
    )"
    :
    : [stack_top]"r"(address)
    : "r1"
    );
}

[[nodiscard]] static inline auto get_stack_remaining()
{
    std::uint32_t* sp;

    asm volatile(R"(
        mov %[sp], sp
        mov sp,    r1
    )"
    : [sp] "=r" (sp)
    :
    :);

    return reinterpret_cast<std::uint32_t*>(MainStackBottom) - sp;
}

template <os::service_call_code_type k_type>
static inline auto service_call(std::size_t size) -> void*
{
    bool  succeess{false};
    void* pointer{nullptr};

    asm volatile(R"(
        dmb
        mov r0, %[param]
        svc %[immediate]
        mov %[pointer],  r0
        mov %[succeess], r1
    )"
    : [pointer]   "=r" (pointer), [succeess]  "=r" (succeess)
    : [immediate]  "I" (k_type),   [param]      "r" (size)
    : "r0", "r1"
    );

    return pointer;
}

template <os::service_call_code_type k_type>
static inline auto service_call(void* pointer) -> bool
{
    bool succeess{false};

    asm volatile(R"(
        dmb
        mov r0, %[pointer]
        svc %[immediate]
        mov %[succeess], r1
    )"
    : [succeess]  "=r" (succeess)
    : [immediate] "I" (k_type), [pointer] "r" (pointer)
    : "r0", "r1"
    );

    return succeess;
}

[[nodiscard, gnu::always_inline, gnu::target("arch=armv7-m")]] static inline std::uint32_t main_stack_pointer()
{
    std::uint32_t value;

    asm volatile("mrs %0, msp" : "=r" (value));

    return value;
}

[[nodiscard, gnu::always_inline]] static inline std::uint32_t process_stack_pointer()
{
    std::uint32_t value;

    asm volatile("mrs %0, psp" : "=r" (value));

    return value;
}

[[gnu::always_inline]] static inline void wait_for_interrupt()
{
    asm volatile("wfi");
}

[[gnu::always_inline]] static inline void wait_for_event()
{
    asm volatile("wfe");
}

[[gnu::always_inline]] static inline void no_operation()
{
    asm volatile("nop");
}

[[maybe_unused, gnu::always_inline]] inline static bool is_in_interrupt_handler()
{
    std::uint32_t result;

    // Get active interrupt vector. Return 0 if none is active.

    asm volatile("mrs %0, ipsr" : "=r" (result));

    return result != 0;
}

} // namespace sys::arch
