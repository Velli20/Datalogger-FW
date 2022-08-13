#pragma once

namespace sys::arch::detail {

template <device_node k_type, std::size_t k_index>
struct interrupt_at
{
    static constexpr auto value = []
    {
        std::size_t i{};

        for ( const auto& [peripheral, interrupt, priority] : sys::hal::k_interrupt_map )
        {
            if ( peripheral != k_type )
            {
                continue;
            }

            if ( i == k_index )
            {
                return std::make_tuple(static_cast<IRQn_Type>(interrupt), static_cast<std::int32_t>(priority));
            }

            i++;
        }

        for ( const auto& [peripheral, interrupt, priority] : sys::hal::k_core_interrupt_table )
        {
            if ( peripheral != k_type )
            {
                continue;
            }

            if ( i == k_index )
            {
                return std::make_tuple(static_cast<IRQn_Type>(interrupt), static_cast<std::int32_t>(priority));
            }

            i++;
        }
    }();
};

template <device_node k_type>
struct interrupt_count
{
    static constexpr std::size_t value = []
    {
        const auto is_type = [=](const auto& interrupt_def) { return std::get<0>(interrupt_def) == k_type; };

        return std::count_if(std::cbegin(sys::hal::k_interrupt_map), std::cend(sys::hal::k_interrupt_map), is_type) +
               std::count_if(std::cbegin(sys::hal::k_core_interrupt_table), std::cend(sys::hal::k_core_interrupt_table), is_type);
    }();
};

} // namespace sys::arch::detail
