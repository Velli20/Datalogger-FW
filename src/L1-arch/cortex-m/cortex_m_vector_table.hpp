#pragma once

#if defined(CONFIG_USE_SEGGER_SYSTEM_VIEW)
#include "FreeRTOS.h"
#endif /* CONFIG_USE_SEGGER_SYSTEM_VIEW */

extern "C" void SEGGER_SYSVIEW_RecordEnterISR(void);
extern "C" void SEGGER_SYSVIEW_RecordExitISR(void);

namespace sys::hal {

void handle_bad_interrupt();

namespace detail {

union vector_address_type
{
    interrupt_handler_type m_function;
    std::uint32_t          m_address;

    constexpr vector_address_type() = default;

    constexpr explicit vector_address_type(std::uint32_t a)
        : m_address{a}
        {}

    constexpr explicit vector_address_type(interrupt_handler_type a)
        : m_function{a}
        {}
};

} // namespace detail

template <std::size_t k_count, auto k_vendor_irq_map>
struct vector_table
{
    private:

    template <auto k_map, std::size_t... Is>
    struct interrupt_configuration_table
    {
        struct interrupt_configuration_type
        {
            std::int32_t           m_table_index;
            interrupt_handler_type m_handler;
        };

        static constexpr interrupt_configuration_type k_interrupt_defination[sizeof...(Is)] =
        {
            []
            {
                constexpr auto k_node      = std::get<0>(k_map[Is]);
                constexpr auto k_interrupt = std::get<1>(k_map[Is]);

                static_assert(sys::arch::concepts::has_interrupt_handler<k_node, k_interrupt> == true);

                if constexpr ( sys::config::system_view::value == true )
                {
                    return interrupt_configuration_type
                    {
                        static_cast<std::int32_t>(k_interrupt) + 16,
                        []()
                        {
                            SEGGER_SYSVIEW_RecordEnterISR();
                            peripheral_type<k_node>:: template interrupt_handler<k_interrupt>();
                            SEGGER_SYSVIEW_RecordExitISR();
                        }
                    };
                }

                else
                {
                    return interrupt_configuration_type
                    {
                        static_cast<std::int32_t>(k_interrupt) + 16, hal::peripheral_type<k_node>:: template interrupt_handler<k_interrupt>
                    };
                }
            }()...
        };
    };

    public:

    detail::vector_address_type vectors[static_cast<std::size_t>(arch::arm::vendor_irqn_type::k_max)] {};

    consteval vector_table()
    {
        constexpr auto k_core_irq_table = [] <typename T, T... Is> (std::integer_sequence<T, Is...>)
        {
            return interrupt_configuration_table<sys::hal::k_core_interrupt_table, Is...>();
        }(std::make_integer_sequence<std::size_t, util::count_of(sys::hal::k_core_interrupt_table)>{});

        constexpr auto k_vendor_irq_table = [] <typename T, T... Is> (std::integer_sequence<T, Is...>)
        {
            return interrupt_configuration_table<k_vendor_irq_map, Is...>();
        }(std::make_integer_sequence<std::size_t, k_count>{});

        // Fill vector table with a default handler.

        std::fill(std::begin(vectors), std::end(vectors), detail::vector_address_type{handle_bad_interrupt});

        // Vector table starts with a Main Stack pointer.

        vectors[0] = detail::vector_address_type{arch::MainStackTop};

        // Fill in core IRQn's.

        for ( const auto& [index, handler] : k_core_irq_table.k_interrupt_defination )
        {
            vectors[index] = detail::vector_address_type{handler};
        }

        // Fill in IRQn's.

        for ( const auto& [index, handler] : k_vendor_irq_table.k_interrupt_defination )
        {
            vectors[index] = detail::vector_address_type{handler};
        }
    }
};

} // namespace sys::hal
