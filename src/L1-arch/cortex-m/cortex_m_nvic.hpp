
#pragma once

namespace sys::arch {

class nvic final
{
    private:

    static constexpr std::uint32_t k_nvic_priority_group_level = 0;

    public:

    static inline auto init() noexcept -> bool
    {
        NVIC_SetPriorityGrouping(k_nvic_priority_group_level /* 4 */);

        return true;
    }

    template <device_node k_type, std::size_t k_index>
    static constexpr auto interrupt_enable() noexcept -> bool
    {
        constexpr auto k_irqn     = std::get<0>(detail::interrupt_at<k_type, k_index>::value);
        constexpr auto k_priority = std::get<1>(detail::interrupt_at<k_type, k_index>::value);

        NVIC_DisableIRQ(k_irqn);
        NVIC_ClearPendingIRQ(k_irqn);

        arch::data_synchronization_barrier();

        const auto priority = NVIC_EncodePriority(k_nvic_priority_group_level, k_priority, 1UL);

        NVIC_SetPriority(k_irqn, priority);
        arch::data_synchronization_barrier();

        NVIC_EnableIRQ(k_irqn);

        // Flush instruction pipeline.
        // http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHJICIE.html

        arch::instruction_synchronization_barrier();

        return true;
    }

    template <device_node k_type> requires (arch::concepts::has_irqn<k_type>)
    static constexpr auto interrupt_enable() noexcept -> bool
    {
        [&] <typename T, T... Is> (std::integer_sequence<T, Is...>)
        {
            (nvic::interrupt_enable<k_type, Is>(), ...);
        }(std::make_integer_sequence<std::size_t, detail::interrupt_count<k_type>::value>{});

        return true;
    }

};

} // namespace sys::arch

