#pragma once

namespace sys::arch {

void startup();

} // namespace sys::arch

namespace sys::hal {

template <>
struct peripheral_type<"/chip/cpu@0"> final
{
    private :

    static inline constinit auto* m_svc_handler    = +[](void*, os::service_call_code_type, os::mode_type) {};
    static inline constinit auto* m_pendsv_handler = +[]() {};

    public:

    static auto on_service_call(arch::concepts::svc_handler auto callback)
    {
        m_svc_handler = callback;
    }

    static auto on_pendable_service_call(arch::concepts::pendable_svc_handler auto callback)
    {
        m_pendsv_handler = callback;
    }

    // Reset handler.

    template <arch::arm::core_irqn_type k_type>
    inline static auto interrupt_handler [[noreturn]] () requires(k_type == arch::arm::core_irqn_type::k_reset_irq)
    {
        arch::startup();

        while ( true )
        {
            arch::no_operation();
        }
    }

    template <arch::arm::core_irqn_type k_type> [[gnu::target("arch=armv7-m")]]
    static constexpr auto interrupt_handler() noexcept requires(k_type == arch::arm::core_irqn_type::k_non_maskable_irq     ||
                                                                k_type == arch::arm::core_irqn_type::k_hard_fault_irq       ||
                                                                k_type == arch::arm::core_irqn_type::k_memory_managment_irq ||
                                                                k_type == arch::arm::core_irqn_type::k_bus_fault_irq        ||
                                                                k_type == arch::arm::core_irqn_type::k_usage_fault_irq)
    {
        // JLink option -excdbg
        // Enables exception debugging. Exceptions on ARM CPUs are handled by exception handlers.
        // In order to do this, a special exception handler is required as follows:

        asm volatile(R"(
            bkpt 10
            bx   lr
        )");
    }

#if 0
    template <arch::arm::core_irqn_type k_type> [[gnu::naked, gnu::target("arch=armv7-m")]]
    static constexpr auto interrupt_handler() noexcept requires(k_type == arch::arm::core_irqn_type::k_non_maskable_irq     ||
                                                                k_type == arch::arm::core_irqn_type::k_hard_fault_irq       ||
                                                                k_type == arch::arm::core_irqn_type::k_memory_managment_irq ||
                                                                k_type == arch::arm::core_irqn_type::k_bus_fault_irq        ||
                                                                k_type == arch::arm::core_irqn_type::k_usage_fault_irq)
    {
        asm volatile(R"(
            mov   r0,     lr
            movs  r1,     #4
            tst   r0,     r1
            mrs   r12,    msp
            beq main_stack_active%=
            mrs   r12,    psp
            main_stack_active%=:
            mov   r0,     r8
            mov   r1,     r9
            mov   r2,     r10
            mov   r3,     r11

            ; Save away r0-r3, r12 and lr_irq for the previous context to the
            ; process stack since they are clobbered here.  Also, save away lr
            ; and spsr_irq since we may swap processes and return to a different
            ; thread.

            push  {r0-r3, lr}        ; Save LR on the stack
            mov   r3,     r12
            push  {r3-r7}
            mov   r0,     sp
            ldr   r1, =%[type]
            ldr   r2, =%[handler]
            bx    r2
            )"
            :
            : [type]    "i" (k_type)
            , [handler] "i" (fault_storage::Process));
    }
#endif

    template <arch::arm::core_irqn_type k_type> requires(k_type == arch::arm::core_irqn_type::k_pending_service_call_irq)
    static constexpr auto interrupt_handler() noexcept
    {
        if ( m_pendsv_handler != nullptr )
        {
            m_pendsv_handler();
        }
    }

    template <arch::arm::core_irqn_type k_type> [[gnu::naked, gnu::optimize("O0")]]
    static constexpr auto interrupt_handler() requires(k_type == arch::arm::core_irqn_type::k_service_call_irq)
    {
        asm volatile(R"(
            tst   LR, #0x4      /* test bit 2 of EXC_RETURN to know if MSP or PSP */
            ite   EQ            /* if zero (equal), came from handler mode */
            mrseq R0, MSP       /* handler mode, stack frame is on MSP (return to main stack) */
            mrsne R0, PSP       /* thread mode, stack frame is on PSP  (return to process stack) */
            tst   LR, #0x8
            ite   EQ
            ldreq R2, =0x0
            ldrne R2, =0x1

            ldr   R1, [R0,#24]  /* Figure out what SVC call number was invoked */
            ldrb  R1, [R1,#-2]  /* grab address of PC from stack frame */

            /* SVC is a two-byte instruction, point to it and read the */
            /* SVC number (lower byte of SCV instruction) */
            /* Branch to corresponding handler. */

            push  {LR}          /* Save LR on the stack */ /* Save EXC_RETURN */
            bl    %[handler]
            isb
            dsb
            pop   {LR}          /* unstack LR */
            bx    LR            /* return from exception (branch to the link register) */
            )"
            :
            :[handler] "g" (service_call_handler)
            : "r0", "r1", "r2");
    }

    private:

    static void service_call_handler(
        void*                      sp,
        os::service_call_code_type type,
        os::mode_type              mode)
    {
        if ( m_svc_handler != nullptr )
        {
            m_svc_handler(sp, type, mode);
        }
    }
};

} // namespace sys::hal
