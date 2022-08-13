#pragma once

namespace sys::hal::armv7m {

enum class errc : std::int32_t
{
    k_no_error,
    k_debug_event,
    k_vector_table_fetch,

    // Usage Faults:

    k_divide_by_zero,                          /* Attempt to execute an SDIV or UDIV instruction with a divisor of 0. */
    k_unaligned_access,                        /* Unaligned memory acceess. */
    k_coprocessor_access,                      /* The processor does not support coprocessor instructions. */
    k_invalid_program_counter_load,            /* Illegal load of EXC_RETURN to the PC. */
    k_invalid_state,                           /* Attempte to execute an instruction that makes illegal use of the EPSR. */
    k_undefined_instruction,                   /* Attempte to execute an undefined instruction. */

    // Bus Faults.

    k_exception_entry_data_bus_error,          /* Bus Fault on stacking for exception entry. */
    k_exception_return_data_bus_error,         /* Bus Fault on unstacking for exception return. */
    k_imprecise_data_bus_error,                /* Asynchronous fault. Faulting address is unknown. */
    k_precise_data_bus_error,                  /* Synchronous fault. Faulting address is written to the BFAR. */
    k_instruction_bus_error,                   /* Bus Fault on instruction prefetching. */

    // Memory Acceess Faults.

    k_exception_entry_memory_managment_error,  /* Memory acceess violation on stacking for exception entry. */
    k_exception_return_memory_managment_error, /* Memory acceess violation on unstacking for exception return. */
    k_data_acceess_violation,                  /* Memory acceess violation during a LDR or STR instruction. */
    k_instruction_acceess_violation,           /* Memory acceess violation during instruction. pre-fetch operation. */
    k_unknown,
};

} // namespace sys::hal::armv7m

template <>
struct sys::is_error_code_enum<sys::hal::armv7m::errc> : std::true_type {};

namespace sys::hal {

static constexpr struct cortex_m_error_category : public sys::error_category
{
    constexpr cortex_m_error_category()
        : error_category{}
        {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override
    {
        return std::string_view{"ARM FAULT"};
    }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<armv7m::errc>(value) )
            {
                case armv7m::errc::k_no_error:
                {
                    return "No error"sv;
                }

                case armv7m::errc::k_debug_event:
                {
                    return "Debug event"sv;
                }

                case armv7m::errc::k_vector_table_fetch:
                {
                    return "Exception during Vector table fetch."sv;
                }

                case armv7m::errc::k_divide_by_zero:
                {
                    return "Attempt to execute an SDIV or UDIV instruction with a divisor of 0."sv;
                }

                case armv7m::errc::k_unaligned_access:
                {
                    return "Unaligned memory acceess."sv;
                }

                case armv7m::errc::k_coprocessor_access:
                {
                    return "The processor does not support coprocessor instructions."sv;
                }

                case armv7m::errc::k_invalid_program_counter_load:
                {
                    return "Illegal load of EXC_RETURN to the PC."sv;
                }

                case armv7m::errc::k_invalid_state:
                {
                    return "Attempte to execute an instruction that makes illegal use of the EPSR."sv;
                }

                case armv7m::errc::k_undefined_instruction:
                {
                    return "Attempt to execute an undefined instruction."sv;
                }

                case armv7m::errc::k_exception_entry_data_bus_error:
                {
                    return "Bus Fault on stacking for exception entry."sv;
                }

                case armv7m::errc::k_exception_return_data_bus_error:
                {
                    return "Bus Fault on unstacking for exception return."sv;
                }

                case armv7m::errc::k_imprecise_data_bus_error:
                {
                    return "Asynchronous fault. Faulting address is unknown."sv;
                }

                case armv7m::errc::k_precise_data_bus_error:
                {
                    return "Synchronous fault. Faulting address is written to the BFAR."sv;
                }

                case armv7m::errc::k_instruction_bus_error:
                {
                    return "Bus Fault on instruction prefetching."sv;
                }

                case armv7m::errc::k_exception_entry_memory_managment_error:
                {
                    return "Memory acceess violation on stacking for exception entry."sv;
                }

                case armv7m::errc::k_exception_return_memory_managment_error:
                {
                    return "Memory acceess violation on unstacking for exception return."sv;
                }

                case armv7m::errc::k_data_acceess_violation:
                {
                    return "Memory acceess violation during a LDR or STR instruction."sv;
                }

                case armv7m::errc::k_instruction_acceess_violation:
                {
                    return "Memory acceess violation during instruction. pre-fetch operation."sv;
                }

                case armv7m::errc::k_unknown:
                {
                    return "Unknown error"sv;
                }
            }
        }

        return {};
    }
} k_cortex_m_error_category{};

[[nodiscard]] constexpr auto make_error_code(armv7m::errc error)
{
    return sys::error_code{error, k_cortex_m_error_category};
}

} // namespace sys::hal
