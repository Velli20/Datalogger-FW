#pragma once

namespace sys::hal {

// Extended interrupts and events controller.

template <>
struct peripheral_type<"/chip/exti">
{
    private:

    static inline constinit auto m_handlers = std::array<external_line_callback, 16 /* FIXME */>{};

    template <std::uint32_t k_line>
    [[nodiscard]] static constexpr auto& get() noexcept
    {
        constexpr auto k_base_address = register_address_of_node<"/chip/exti">::value;

        return mmio::place_at<ExtendedLineLayout<k_line>>(k_base_address);
    }

    public:

    static auto init() noexcept -> bool
    {
        return arch::nvic::interrupt_enable<"/chip/exti">();
    }

    template <device_node k_node> requires (detail::instance_of_gpio_node<k_node>::value)
    static constexpr auto enable_external_interrupt(const external_line_callback callback) noexcept -> bool
    {
        constexpr auto k_pin = unit_sub_address_of_node<k_node>::value;

        static_assert(k_pin >= 0 && k_pin <= 15);

        m_handlers[k_pin] = callback;

        hal::peripheral_type<"/chip/system-configuration">::set_external_line_interrupt_source<k_node>();

        get<k_pin>().RisingTriggerSelection1.Set();
        get<k_pin>().InterruptMask1.Set();

        return true;
    }

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_external_line0)
    static constexpr void interrupt_handler() noexcept
    {
        do_handle_interrupt<0u>();
    }

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_external_line1)
    static constexpr void interrupt_handler() noexcept
    {
        do_handle_interrupt<1u>();
    }

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_external_line2)
    static constexpr void interrupt_handler() noexcept
    {
        do_handle_interrupt<2u>();
    }

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_external_line3)
    static constexpr void interrupt_handler() noexcept
    {
        do_handle_interrupt<3u>();
    }

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_external_line4)
    static constexpr void interrupt_handler() noexcept
    {
        do_handle_interrupt<4u>();
    }

    private:

    template <std::uint32_t k_line>
    static constexpr void do_handle_interrupt() noexcept
    {
        get<k_line>().PendingInterruptFlag1.Write(1ul);

        if ( m_handlers[k_line] != nullptr )
        {
            m_handlers[k_line]();
        };
    }

    template <std::uint32_t k_begin, std::uint32_t k_end>
    static constexpr void interrupt_handler_for_range() noexcept
    {
        static_assert(k_end > k_begin);

        // Check and handle External line interrupts from 'k_begin' offset to 'end'.

        [] <typename T, T... Is> (std::integer_sequence<T, Is...>)
        {
            ([&]
            {
                if ( get<k_begin + Is>().PendingInterruptFlag1.Read() != 0ul )
                {
                    do_handle_interrupt<k_begin + Is>();
                }
            }(), ...);
        }(std::make_integer_sequence<std::uint32_t, k_end - k_begin>{});
    }

    public:

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_external_line9to5)
    static constexpr void interrupt_handler() noexcept
    {
        interrupt_handler_for_range<5ul, 9ul>();
    }

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_external_line15to10)
    static constexpr void interrupt_handler() noexcept
    {
        interrupt_handler_for_range<10ul, 15ul>();
    }
};

} // namespace sys::hal
