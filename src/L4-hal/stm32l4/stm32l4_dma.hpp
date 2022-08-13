#pragma once

namespace sys::hal {

namespace detail {

template <device_node k_node>
struct traits
{
    private:

    static constexpr auto value = []()
    {
        constexpr const std::tuple<device_node, device_node, device_node> k_dma_peripheral_map[] =
        {
            { device_node{"/chip/dma@1"}, device_node{"/chip/dma1-channel@1"}, device_node{"/chip/dma-mux-channel@0"}  },
            { device_node{"/chip/dma@1"}, device_node{"/chip/dma1-channel@2"}, device_node{"/chip/dma-mux-channel@1"}  },
            { device_node{"/chip/dma@1"}, device_node{"/chip/dma1-channel@3"}, device_node{"/chip/dma-mux-channel@2"}  },
            { device_node{"/chip/dma@1"}, device_node{"/chip/dma1-channel@4"}, device_node{"/chip/dma-mux-channel@3"}  },
            { device_node{"/chip/dma@1"}, device_node{"/chip/dma1-channel@5"}, device_node{"/chip/dma-mux-channel@4"}  },
            { device_node{"/chip/dma@1"}, device_node{"/chip/dma1-channel@6"}, device_node{"/chip/dma-mux-channel@5"}  },
            { device_node{"/chip/dma@1"}, device_node{"/chip/dma1-channel@7"}, device_node{"/chip/dma-mux-channel@6"}  },

            { device_node{"/chip/dma@2"}, device_node{"/chip/dma2-channel@1"}, device_node{"/chip/dma-mux-channel@7"}  },
            { device_node{"/chip/dma@2"}, device_node{"/chip/dma2-channel@2"}, device_node{"/chip/dma-mux-channel@8"}  },
            { device_node{"/chip/dma@2"}, device_node{"/chip/dma2-channel@3"}, device_node{"/chip/dma-mux-channel@9"}  },
            { device_node{"/chip/dma@2"}, device_node{"/chip/dma2-channel@4"}, device_node{"/chip/dma-mux-channel@10"} },
            { device_node{"/chip/dma@2"}, device_node{"/chip/dma2-channel@5"}, device_node{"/chip/dma-mux-channel@11"} },
            { device_node{"/chip/dma@2"}, device_node{"/chip/dma2-channel@6"}, device_node{"/chip/dma-mux-channel@12"} },
            { device_node{"/chip/dma@2"}, device_node{"/chip/dma2-channel@7"}, device_node{"/chip/dma-mux-channel@13"} },
        };

        for ( const auto& item : k_dma_peripheral_map )
        {
            if ( k_node.m_node_name == std::get<1>(item).m_node_name && k_node.m_unit_address == std::get<1>(item).m_unit_address  )
            {
                return item;
            }
        }
    }();

    public:

    static constexpr auto dma_type = std::get<0>(value);
    static constexpr auto mux_type = std::get<2>(value);
};

} // namespace detail

template <device_node k_node> requires (detail::instance_of_dma_channel_node<k_node>::value)
struct peripheral_type<k_node> final
{
    private:

    static constexpr auto k_channel_index = unit_address_of_node<k_node>::value;
    static constexpr auto k_dma_config    = std::get<dma_channel_phy_config_type>(hal::peripheral_of(k_node).config);

    [[nodiscard]] static constexpr auto dma_channel() noexcept -> DmaChannelLayout&
    {
        constexpr auto k_channel_base = register_address_of_node<k_node>::value;

        return mmio::place_at<DmaChannelLayout>(k_channel_base);
    }

    [[nodiscard]] static constexpr auto dma() noexcept -> DmaLayout<k_channel_index>&
    {
        constexpr auto k_dma_base = register_address_of_node<detail::traits<k_node>::dma_type>::value;

        return mmio::place_at<DmaLayout<k_channel_index>>(k_dma_base);
    }

    [[nodiscard]] static constexpr auto dma_mux_status() noexcept -> DmaMuxChannelStatusLayout<k_channel_index>&
    {
        constexpr auto k_dma_mux_status_base = register_address_of_node<"/chip/dma-mux-channel-status@0">::value;

        return mmio::place_at<DmaMuxChannelStatusLayout<k_channel_index>>(k_dma_mux_status_base);
    }

    [[nodiscard]] static constexpr auto dma_mux() noexcept -> DmaMuxChannelLayout&
    {
        constexpr auto k_dma_mux_base = register_address_of_node<detail::traits<k_node>::mux_type>::value;

        return mmio::place_at<DmaMuxChannelLayout>(k_dma_mux_base);
    }

    [[nodiscard]] static constexpr auto is_enabled() noexcept
    {
        return dma_channel().Configuration.Read<DmaChannelLayout::Configuration::ChannelEnable>() != 0ul ? true : false;
    }

    public:

    static constexpr void reset() noexcept
    {
        using namespace std::chrono_literals;

        // Clear channel enabled bit.

        dma_channel().Configuration.Clear<DmaChannelLayout::Configuration::ChannelEnable>();

        // Wait until DMA stream is disabled. This might take a while if there is transfer ongoing.

        if ( util::busy_wait(20000ms, [] { return is_enabled() ? false : true; }) == false )
        {
            return;
        }

        // Disable channel and reset configuration.

        dma_channel().Configuration.Clear();

        // Reset data count.

        dma_channel().DataCount.Clear();

        // Clear peripheral address.

        dma_channel().PeripheralAddress.Clear();

        // Clear memory address.

        dma_channel().MemoryAddress.Clear();
    }

    struct interrupt
    {
        [[nodiscard]] static constexpr bool is_transfer_complete() noexcept
        {
            return dma().InterruptStatus.TransferComplete.Read() != 0ul && dma_channel().Configuration.Read<DmaChannelLayout::Configuration::TransferCompleteInterrupt>() != 0ul;
        }

        [[nodiscard]] static constexpr bool is_half_transfer_complete() noexcept
        {
            return dma().InterruptStatus.HalfTransfer.Read() != 0ul && dma_channel().Configuration.Read<DmaChannelLayout::Configuration::HalfTransferInterrupt>() != 0ul;
        }

        [[nodiscard]] static constexpr bool is_transfer_error() noexcept
        {
            return dma().InterruptStatus.TransferError.Read() != 0ul && dma_channel().Configuration.Read<DmaChannelLayout::Configuration::TransferErrorInterrupt>() != 0ul;
        }

        struct enable
        {
            static constexpr auto transfer_complete(bool enable = true) noexcept
            {
                dma_channel().Configuration.Write<DmaChannelLayout::Configuration::TransferCompleteInterrupt>(enable ? 1ul : 0ul);
            }

            static constexpr auto half_transfer_complete(bool enable = true) noexcept
            {
                dma_channel().Configuration.Write<DmaChannelLayout::Configuration::HalfTransferInterrupt>(enable ? 1ul : 0ul);
            }

            static constexpr auto transfer_error(bool enable = true) noexcept
            {
                dma_channel().Configuration.Write<DmaChannelLayout::Configuration::TransferErrorInterrupt>(enable ? 1ul : 0ul);
            }
        };

        static constexpr auto clear() noexcept
        {
            // Clear all flags.

            dma().InterruptFlagClear.GlobalInterrupt.Set();
#if 0
            // Handle Half Transfer Complete Interrupt.

            if ( is_half_transfer_complete() )
            {
                // Disable the half transfer interrupt if the DMA mode is not CIRCULAR.

                if ( dma_channel().Configuration.CircularMode.Read() == dma_mode_type::k_normal )
                {
                    dma_channel().Configuration.HalfTransferInterrupt.Clear();
                }

                // Clear the half transfer complete flag.

                dma().InterruptFlagClear.HalfTransfer.Set();
            }

            // Handle Transfer Complete Interrupt.

            else if ( is_transfer_complete() )
            {
                if ( dma_channel().Configuration.CircularMode.Read() == dma_mode_type::k_normal )
                {
                    dma_channel().Configuration.TransferCompleteInterrupt.Clear();
                    dma_channel().Configuration.TransferErrorInterrupt.Clear();
                }

                // Clear the transfer complete flag.

                dma().InterruptFlagClear.TransferComplete.Set();
            }

            // Handle Transfer Error Interrupt.

            else if ( is_transfer_error() )
            {
                // Disable all DMA interrupts.

                dma_channel().Configuration.Clear<DmaChannelLayout::Configuration::HalfTransferInterrupt>();
                dma_channel().Configuration.Clear<DmaChannelLayout::Configuration::TransferCompleteInterrupt>();
                dma_channel().Configuration.Clear<DmaChannelLayout::Configuration::TransferErrorInterrupt>();

                // Clear all flags.

                dma().InterruptFlagClear.GlobalInterrupt.Set();
            }
#endif
        }
    };

    public:

    static constexpr auto init() requires (k_dma_config.direction == dma_direction_type::k_peripheral_to_memory)
    {
        hal::peripheral_type<"/chip/rcc">::enable<"/chip/dma-mux">();
        hal::peripheral_type<"/chip/rcc">::enable<detail::traits<k_node>::dma_type>();

        reset();

        dma_channel().Configuration
                    .Edit()
                    .Clear<DmaChannelLayout::Configuration::MemoryToMemory>       ()
                    .Clear<DmaChannelLayout::Configuration::DataTransferDirection>()
                    .Write<DmaChannelLayout::Configuration::MemorySize>           (k_dma_config.data_size)
                    .Write<DmaChannelLayout::Configuration::PeripheralSize>       (k_dma_config.data_size)
                    .Write<DmaChannelLayout::Configuration::CircularMode>         (k_dma_config.mode)
                    .Set  <DmaChannelLayout::Configuration::MemoryIncrementMode>  ()
                    .Write<DmaChannelLayout::Configuration::PriorityLevel>        (k_dma_config.priority)
                    .Save();

#if defined(DMAMUX1)

        // Set peripheral request to DMAMUX channel.

        dma_mux().Configuration.DmaRequestId.Write(k_dma_config.request);

        // Clear the DMAMUX synchro overrun flag.

        dma_mux_status().ClearFlag.SynchronizationOverrunEvent.Clear();

        // if(((hdma->init.Request > 0U) && (hdma->init.Request <= DMA_REQUEST_GENERATOR3)))
        // {
        //   /* Initialize parameters for DMAMUX request generator :
        //      DMAmuxRequestGen, DMAmuxRequestGenStatus and DMAmuxRequestGenStatusMask
        //   */
        //   DMA_CalcDMAMUXRequestGenBaseAndMask(hdma);

        //   /* Reset the DMAMUX request generator register*/
        //   hdma->DMAmuxRequestGen->RGCR = 0U;

        //   /* Clear the DMAMUX request generator overrun flag */
        //   hdma->DMAmuxRequestGenStatus->RGCFR = hdma->DMAmuxRequestGenStatusMask;
        // }

#endif /* DMAMUX1 */

        arch::nvic::interrupt_enable<k_node>();

        return true;
    }

    template <arch::arm::vendor_irqn_type k_type>
    static constexpr void interrupt_handler() noexcept
    {
        arch::scoped_interrupt_lock guard;

        if ( interrupt::is_transfer_complete() )
        {
            m_overflow_count.fetch_add(1ul);
        }

        interrupt::clear();
    }

    [[nodiscard]] static constexpr auto read_position() noexcept
    {
        return dma_channel().DataCount.Read();
    }

    [[nodiscard]] static constexpr auto overflow_count() noexcept
    {
        return m_overflow_count.load();
    }

    [[maybe_unused]] static constexpr auto start(
        std::uint32_t peripheral_address,
        std::uint32_t memory_address,
        std::uint32_t data_length
    ) noexcept
    {
        if ( is_enabled() )
        {
            return false;
        }

        // Set the address of the peripheral register to or from which data is to be transferred.

        dma_channel().PeripheralAddress.Write(peripheral_address);

        // Set the memory address to or from which data is to be transferred.

        dma_channel().MemoryAddress.Write(memory_address);

        // Set data count.

        dma_channel().DataCount.Write(data_length);

        // Enable.

        dma_channel().Configuration.Set<DmaChannelLayout::Configuration::ChannelEnable>();

        return true;
    }

    private:

    static inline constinit std::atomic<std::uint32_t> m_overflow_count{};
};

} // namespace sys::hal
