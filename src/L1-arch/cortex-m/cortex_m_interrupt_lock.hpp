#pragma once

namespace sys::arch {

struct scoped_interrupt_lock final
{
    [[gnu::always_inline]] scoped_interrupt_lock()
        : m_cpsr{ __get_PRIMASK() }
    {
        arch::disable_interrupts();
    }

    [[gnu::always_inline]] ~scoped_interrupt_lock()
    {
        __set_PRIMASK(m_cpsr);
    }

    private:

    std::uint32_t m_cpsr{};
};

} // namespace sys::arch
