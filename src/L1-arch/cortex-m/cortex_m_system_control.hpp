#pragma once

namespace sys::hal {

struct system_control_block final
{
    private:

    static constexpr auto access = []() -> SystemControlBlockLayout&
    {
        constexpr auto k_base_address = register_address_of_node<"/chip/cortex-m-scb">::value;

        return mmio::place_at<SystemControlBlockLayout>(k_base_address);
    };

    public:

    [[nodiscard]] static constexpr auto fault_code() noexcept
    {
        if ( access().HardFaultStatus.DebugEvent.Read() != 0ul )
        {
            return armv7m::errc::k_debug_event;
        }

        if ( access().HardFaultStatus.VectorTableHardFault.Read() != 0ul )
        {
            return armv7m::errc::k_vector_table_fetch;
        }

        constexpr std::tuple<armv7m::errc, std::uint32_t> k_bit_lookup[] =
        {
            { armv7m::errc::k_divide_by_zero,                          SCB_CFSR_DIVBYZERO_Msk    },
            { armv7m::errc::k_unaligned_access,                        SCB_CFSR_UNALIGNED_Msk    },
            { armv7m::errc::k_coprocessor_access,                      SCB_CFSR_NOCP_Msk         },
            { armv7m::errc::k_invalid_program_counter_load,            SCB_CFSR_INVPC_Msk        },
            { armv7m::errc::k_invalid_state,                           SCB_CFSR_INVSTATE_Msk     },
            { armv7m::errc::k_undefined_instruction,                   SCB_CFSR_UNDEFINSTR_Msk   },
            { armv7m::errc::k_exception_entry_data_bus_error,          SCB_CFSR_STKERR_Msk       },
            { armv7m::errc::k_exception_return_data_bus_error,         SCB_CFSR_UNSTKERR_Msk     },
            { armv7m::errc::k_imprecise_data_bus_error,                SCB_CFSR_IMPRECISERR_Msk  },
            { armv7m::errc::k_precise_data_bus_error,                  SCB_CFSR_PRECISERR_Msk    },
            { armv7m::errc::k_instruction_bus_error,                   SCB_CFSR_IBUSERR_Msk      },
            { armv7m::errc::k_exception_entry_memory_managment_error,  SCB_CFSR_MSTKERR_Msk      },
            { armv7m::errc::k_exception_return_memory_managment_error, SCB_CFSR_MUNSTKERR_Msk    },
            { armv7m::errc::k_data_acceess_violation,                  SCB_CFSR_DACCVIOL_Msk     },
            { armv7m::errc::k_instruction_acceess_violation,           SCB_CFSR_IACCVIOL_Msk     }
        };

        for ( auto reason = access().ConfigurableFaultStatus.Read(); const auto& [code, mask] : k_bit_lookup )
        {
            if ( reason & mask )
            {
                return code;
            }
        }

        return armv7m::errc::k_unknown;
    }

    [[nodiscard]] static constexpr auto is_in_interrupt_handler()
    {
        return access().InterruptControlAndState.ActiveVector.Read() != 0ul;
    }

    [[nodiscard]] static constexpr auto active_interrupt_vector()
    {
        return access().InterruptControlAndState.ActiveVector.Read();
    }

    template <stack_alignment_type k_aligment>
    static constexpr auto set_exception_entry_stack_alignment()
    {
        access().ConfigurationControl.stack_alignment_type.Write(static_cast<std::uint32_t>(k_aligment));
    }

    static constexpr auto ignore_data_bus_faults(bool ignore = true)
    {
        access().ConfigurationControl.BusFaultHandlerIgnore.Write(ignore ? 1ul : 0ul);
    }

    static constexpr auto fault_on_divide_by_zero(bool fault = true)
    {
        access().ConfigurationControl.DivideByZeroTrap.Write(fault ? 1ul : 0ul);
    }

    static constexpr auto fault_on_unaligned_access(bool fault = true)
    {
        access().ConfigurationControl.UnalignedAccessTrap.Write(fault ? 1ul : 0ul);
    }

    static constexpr auto enable_unprivileged_software_interrupts(bool enable = true)
    {
        access().ConfigurationControl.UnprivilegedSoftwareInterruptTrigger.Write(enable ? 1ul : 0ul);
    }

    static constexpr auto enable_unexceptional_thread_mode_entry(bool enable = true)
    {
        access().ConfigurationControl.ThreadModeEntry.Write(enable ? 1ul : 0ul);
    }

    template <coprocessor_access_type k_access_level> /* [[gnu::target("fpu=fpv4-sp-d16")]] */
    static constexpr auto set_coprosessor_access_level()
    {
        access().CoprocessorAccessControl.Coprocessor10Access.Write(static_cast<std::uint32_t>(k_access_level));
        access().CoprocessorAccessControl.Coprocessor11Access.Write(static_cast<std::uint32_t>(k_access_level));
    }

    private:

    system_control_block() = delete;
};

} // namespace sys::hal
