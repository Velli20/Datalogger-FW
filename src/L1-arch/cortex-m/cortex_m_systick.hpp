#pragma once

namespace sys::hal {

template <>
struct peripheral_type<"/chip/cortex-m-systick"> final
{
    private:

    static constexpr auto k_configuration = std::get<sys_tick_config_type>(hal::peripheral_of("/chip/cortex-m-systick").config);

    [[nodiscard]] static constexpr auto& get() noexcept
    {
        constexpr auto k_base_address = register_address_of_node<"/chip/cortex-m-systick">::value;

        return mmio::place_at<SysTickLayout>(k_base_address);
    }

    public:

    static constexpr auto init() noexcept
    {
        using namespace sys::units::literals;

        static_assert(k_configuration.tick_frequency > 0_Hz, "Invalid SysTick configuration");

        // NVIC configuration for SysTick.

        arch::nvic::interrupt_enable<"/chip/cortex-m-systick">();

        const auto ticks = SystemCoreClock / k_configuration.tick_frequency;
        if ( ticks >= 1ul << 24ul /* SysTick_LOAD_RELOAD_Msk */ )
        {
            return false;
        }

        // A write of any value clears the field to 0.

        get().CurrentValue.Write(0ul);

        // Set counter reload value.

        get().ReloadValue.Write(ticks - 1ul);

        // Set SysTick source, enable interrupt generation and start SysTick.

        using REG = decltype(SysTickLayout::Control);

        get().Control
             .Edit()
             .Write<REG::Source>(k_configuration.source)
             .Set  <REG::ExecptionRequestEnable>()
             .Set  <REG::CounterEnable>()
             .Save();

        return true;
    }

    template <arch::arm::core_irqn_type k_type> requires(k_type == arch::arm::core_irqn_type::k_system_tick_irq)
    static constexpr auto interrupt_handler() noexcept
    {
        // Increment tick count.

        m_tick_count.fetch_add(1ul);

        // Notify tick callback (scheduler).

        if ( m_callback != nullptr )
        {
            m_callback();
        }
    }

    [[nodiscard]] static inline auto now() noexcept
    {
        return m_tick_count.load();
    }

    static inline auto on_tick(sys::arch::concepts::systick_handler auto handler) noexcept
    {
        m_callback = handler;
    }

    private:

    static inline constinit auto m_callback   = interrupt_handler_type{};
    static inline constinit auto m_tick_count = std::atomic<std::uint32_t>{};
};

} // namespace sys::hal
