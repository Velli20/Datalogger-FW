#pragma once

namespace sys::hal {

struct fault_storage
{
    private:

    struct exception_frame_type
    {
        std::uint32_t r0;
        std::uint32_t r1;
        std::uint32_t r2;
        std::uint32_t r3;
        std::uint32_t r12;
        std::uint32_t lr;
        std::uint32_t pc;
        std::uint32_t xpsr;
    };

    static_assert(sizeof(exception_frame_type) == 8 * 4);

    struct registers_state_type
    {
        exception_frame_type* exception_frame;
        std::uint32_t         r4;
        std::uint32_t         r5;
        std::uint32_t         r6;
        std::uint32_t         r7;
        std::uint32_t         r8;
        std::uint32_t         r9;
        std::uint32_t         r10;
        std::uint32_t         r11;
        std::uint32_t         exc_return;
    };

    static_assert(sizeof(registers_state_type) == 10 * 4);

    enum register_type : std::uint32_t
    {
        k_r0                    = 0, /* Argument / scratch register */
        k_r1                    = 1, /* Argument / scratch register */
        k_r2                    = 2, /* Argument / scratch register */
        k_r3                    = 3, /* Argument / scratch register */
        k_r4                    = 4,
        k_r5                    = 5,
        k_r6                    = 6,
        k_r7                    = 7,
        k_r8                    = 8,
        k_r9                    = 9,
        k_r10                   = 10,
        k_r11                   = 11,
        k_r12                   = 12, /* The Intra-Procedure-call scratch register */
        k_stack_pointer         = 13, /* Active stack pointer */
        k_link                  = 14, /* Link register */
        k_program_counter       = 15, /* Program Counter */
        k_program_status        = 16, /* Program Status register */
        k_main_stack_pointer    = 17, /* Program Status register */
        k_process_stack_pointer = 18, /* Program Status register */
        k_first                 = k_r0,
        k_count                 = 19,
    };

    struct crash_context_type
    {
        std::uint32_t m_registers[register_type::k_count];
    };

    static constexpr const char* get_register_name(register_type reg)
    {
        {
            constexpr std::tuple<const char*, register_type> k_register_map[] =
            {
                { "R0",                        register_type::k_r0                    },
                { "R1",                        register_type::k_r1                    },
                { "R2",                        register_type::k_r2                    },
                { "R3",                        register_type::k_r3                    },
                { "R3",                        register_type::k_r3                    },
                { "R4",                        register_type::k_r4                    },
                { "R5",                        register_type::k_r5                    },
                { "R6",                        register_type::k_r6                    },
                { "R7",                        register_type::k_r7                    },
                { "R8",                        register_type::k_r8                    },
                { "R9",                        register_type::k_r9                    },
                { "R10",                       register_type::k_r10                   },
                { "R11",                       register_type::k_r11                   },
                { "R12 (scratch)",             register_type::k_r12                   },
                { "Active stack pointer",      register_type::k_stack_pointer         },
                { "R14 (Link/Return Address)", register_type::k_link                  },
                { "Program counter",           register_type::k_program_counter       },
                { "Program status",            register_type::k_program_status        },
                { "Main stack pointer",        register_type::k_main_stack_pointer    },
                { "Process stack pointer",     register_type::k_process_stack_pointer }
            };

            for ( const auto& [name, item] : k_register_map )
            {
                if ( item == reg )
                {
                    return name;
                }
            }
        }

        return nullptr;
    }

    static void report(const crash_context_type& context)
    {
        constexpr const char k_header[]  = "\t|   mmio::register_type       |   Value    |";
        constexpr const char k_divider[] = "\t+------------------+------------+";
        constexpr const char k_format[]  = "\t| %16.16s | 0x%08X |";

        auto os = log::error();

        os << log::endl;
        os << k_header  << log::endl;
        os << k_divider << log::endl;

        for ( std::uint32_t i = register_type::k_first; i < register_type::k_count; ++i )
        {
            os << log::fmt(k_format, get_register_name(static_cast<register_type>(i)), context.m_registers[i]) << log::endl;
        }
    }

    public:

    static void process(
        const registers_state_type* regs,
        arch::arm::vendor_irqn_type exeption
    )
    {
        static_cast<void>(exeption);

        const bool fpu_stack_space_rsvd   = ((regs->exc_return & (1 << 4)) == 0);
        const bool stack_alignment_forced = ((regs->exception_frame->xpsr & (1 << 9)) != 0);

        auto sp_prior_to_exception = reinterpret_cast<std::uint32_t>(regs->exception_frame) + (fpu_stack_space_rsvd ? 0x68 : 0x20);

        if ( stack_alignment_forced )
        {
            sp_prior_to_exception += 0x4;
        }

        crash_context_type context{};

        const bool msp_was_active = (regs->exc_return & (1 << 2)) == 0;

        context.m_registers[register_type::k_r0]                    = regs->exception_frame->r0;
        context.m_registers[register_type::k_r1]                    = regs->exception_frame->r1;
        context.m_registers[register_type::k_r2]                    = regs->exception_frame->r2;
        context.m_registers[register_type::k_r3]                    = regs->exception_frame->r3;
        context.m_registers[register_type::k_r4]                    = regs->r4;
        context.m_registers[register_type::k_r5]                    = regs->r5;
        context.m_registers[register_type::k_r6]                    = regs->r6;
        context.m_registers[register_type::k_r7]                    = regs->r7;
        context.m_registers[register_type::k_r8]                    = regs->r8;
        context.m_registers[register_type::k_r9]                    = regs->r9;
        context.m_registers[register_type::k_r10]                   = regs->r10;
        context.m_registers[register_type::k_r11]                   = regs->r11;
        context.m_registers[register_type::k_r12]                   = regs->exception_frame->r12;
        context.m_registers[register_type::k_stack_pointer]         = sp_prior_to_exception;
        context.m_registers[register_type::k_link]                  = regs->exception_frame->lr;
        context.m_registers[register_type::k_program_counter]       = regs->exception_frame->pc;
        context.m_registers[register_type::k_program_status]        = regs->exception_frame->xpsr;
        context.m_registers[register_type::k_main_stack_pointer]    =  msp_was_active ? sp_prior_to_exception : arch::main_stack_pointer();
        context.m_registers[register_type::k_process_stack_pointer] = !msp_was_active ? sp_prior_to_exception : arch::process_stack_pointer();

        report(context);

        if ( CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk )
        {
            arch::break_point<255>();
        }

        //NVIC_SystemReset();

        while ( true )
        {
            __NOP();
        }
    }
};

} // namespace sys::hal
