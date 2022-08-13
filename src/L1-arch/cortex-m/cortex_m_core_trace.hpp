#pragma once

namespace sys::hal {

template <>
struct peripheral_type<"/chip/cortex-m-debug"> final
{
    private:

    inline static auto& Get()
    {
        constexpr auto k_base_address = register_address_of_node<"/chip/cortex-m-debug">::value;

        return mmio::place_at<CoreDebugLayout>(k_base_address);
    }

    public:

    inline static auto TraceEnable(bool enable)
    {
        Get().DebugAndMonitorCtrl.Write<CoreDebugLayout::DebugAndMonitorCtrl::TraceEnable>(enable ? 1ul : 0ul);
    }
};

template <>
struct peripheral_type<"/chip/cortex-m-dwt"> final
{
    private:

    static constexpr auto kMaxMatchPoints = std::size_t{4};

    struct WatchPoint
    {
        data_watch_point_callback_type callback;
        void*                  user_data;
    };

    inline static DataWatchPointAndTraceLayout& Get()
    {
        constexpr auto k_base_address = register_address_of_node<"/chip/cortex-m-dwt">::value;

        return mmio::place_at<DataWatchPointAndTraceLayout>(k_base_address);
    }

    public:

    template <data_watch_point_type k_watch_point, data_watch_point_function_type kFunction>
    static constexpr auto SetDataWatchPoint(std::uint32_t address, const data_watch_point_callback_type callback, void* user_data = nullptr)
    {
        constexpr std::uint32_t kId = static_cast<std::uint32_t>(k_watch_point);

        if ( ((DWT->CTRL & DWT_CTRL_NUMCOMP_Msk) >> DWT_CTRL_NUMCOMP_Pos) < (static_cast<std::uint32_t>(k_watch_point) + 1) )
        {
            log::error() << "Watchpoint " << static_cast<std::uint32_t>(k_watch_point) << " not supported" << log::endl;
            // return false;
        }

        /* Use comparator 0, R/W access check */
        DWT->COMP0 = address;

        DWT->FUNCTION0 = (0x7 << DWT_FUNCTION_FUNCTION_Pos) & DWT_FUNCTION_FUNCTION_Msk;

        /* Set mask according to the desired size */
        DWT->MASK0 = 0x5;

        // Assert that DWT supports n-number of counters.

        const auto count = Get().Control.Read<DataWatchPointAndTraceLayout::Control::ComparatorCount>();
        if ( kId >= count )
        {
            log::error() << "Watchpoint " << static_cast<std::uint32_t>(k_watch_point) << " not supported" << log::endl;
            // return false;
        }

        // Get().WatchPoint[kId].Comparator.Write(address);

        // Set action.

        // Get().WatchPoint[kId].Function.Write<DataWatchPointAndTraceLayout::FunctionField::Action>(kFunction);

        // Set number of bits to ignore when performing a comparison.

        /* C1-731:

            Maximum size of the address comparison mask:

            To find the maximum size of the address comparison mask for comparator n,
            software must write 0b11111 to the DWT_MASKn.MASK field, and then read the
            DWT_MASKn register. The value returned in the MASK field is the maximum mask size
        */

        // Get().WatchPoint[kId].Mask.Write(0x5 /* 0x1F */);

        m_watch_points[kId] = { callback, user_data };

        return true;
    }

    static auto IsCycleCounterSupported()
    {
        return Get().Control.Read<DataWatchPointAndTraceLayout::Control::CycleCounterSupported>() == 1ul ? true : false;
    }

    static auto SetCycleCounterValue(const std::uint32_t value)
    {
        Get().CycleCounter.Write(value);
    }

    static auto GetCycleCounterValue()
    {
        // Note that this is typically 2x the CPU frequency as it counts on rising and falling edges.

        return Get().CycleCounter.Read();
    }

    static auto EnableCycleCounter(const bool enable = true)
    {
        hal::peripheral_type<"/chip/cortex-m-debug">::TraceEnable(true);

        SetCycleCounterValue(0ul);

        Get().Control.Write<DataWatchPointAndTraceLayout::Control::CycleCounterEnable>(enable ? 1ul : 0ul);
    }

    template <arch::arm::core_irqn_type k_type> [[gnu::naked, gnu::target("arch=armv7-m")]]
    static constexpr auto interrupt_handler() noexcept requires(k_type == arch::arm::core_irqn_type::k_debug_monitor_irq)
    {
        // log::error() << "debug_monitor" << log::endl;

        constexpr auto k_implementation = [](void* sp)
        {
            log::error() << "... debug_monitor impl ..." << log::endl;

            static_cast<void>(sp);

            for ( std::size_t i{}; i < kMaxMatchPoints; ++i )
            {
                if ( Get().WatchPoint[i].Function.Read<DataWatchPointAndTraceLayout::FunctionField::Matched>() != 0b1 )
                    continue;

                if ( auto [callback, data] = m_watch_points.at(i); callback != nullptr )
                {
                    callback(data);
                }
            }
        };

        asm volatile
        (
            R"(
            mov   r0, lr
            movs  r1, #4
            tst   r0, r1
            ite   eq
            mrseq r0, msp
            mrsne r0, psp
            bl    %[handler]
            )"
            :
            : [handler] "g" (k_implementation)
            : "r0", "r1"
        );
    }

    private:

    static inline constinit auto m_watch_points = std::array<WatchPoint, kMaxMatchPoints>{{nullptr, nullptr}};
};

} // namespace sys::hal
