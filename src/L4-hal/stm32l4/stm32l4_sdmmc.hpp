

#pragma once

namespace sys::hal {

template <>
struct peripheral_type<"/chip/sdmmc@1"> final
{
    static constexpr auto k_operation_timeout_irq = std::chrono::milliseconds(40);
    static constexpr auto k_operation_timeout     = std::chrono::milliseconds(30000);
    static constexpr auto k_node                  = device_node{"/chip/sdmmc@1"};

    public:

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_sdmmc_global)
    static auto interrupt_handler() noexcept
    {
        if ( __HAL_SD_GET_FLAG(m_card_handle.get(), SDMMC_FLAG_DATAEND) != RESET )
        {
            if ( ((m_card_handle.Context & SD_CONTEXT_READ_SINGLE_BLOCK) != 0U) || ((m_card_handle.Context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != 0U) )
            {
                m_read_complete_semaphore.release();
            }

            else
            {
                m_write_complete_semaphore.release();
            }
        }

        HAL_SD_IRQHandler(m_card_handle);
    }

    [[nodiscard]] static auto is_ready() noexcept
    {
        if ( m_initialized.load() == false )
        {
            return false;
        }

        if ( HAL_SD_GetCardState(m_card_handle) == HAL_SD_CARD_TRANSFER )
        {
            return true;
        }

        return false;
    }

    template <typename T>
    [[maybe_unused]] static auto read_blocks(
        const std::span<T> buffer,
        std::uint32_t      block_address
    ) noexcept -> std::size_t
    {
        if ( buffer.empty() )
        {
            return {};
        }

        if ( is_ready() == false )
        {
            debug_assert(false);
            return {};
        }

        std::lock_guard<platform::mutex> lock{m_read_lock};

        // Read block(s) in blocking mode if called from the interrupt context (USB MCS).

        if ( arch::is_in_interrupt_handler() )
        {
            const auto hal_rc = HAL_SD_ReadBlocks(m_card_handle, std::bit_cast<std::uint8_t*>(buffer.data()), block_address, buffer.size(), k_operation_timeout_irq.count());
            if ( hal_rc != HAL_OK )
            {
                m_error = make_error_code(m_card_handle.error());
                return {};
            }

            return buffer.size();
        }

        // Otherwise read block(s) in DMA transfer mode.

        {
            const auto hal_rc = HAL_SD_ReadBlocks_DMA(m_card_handle, std::bit_cast<std::uint8_t*>(buffer.data()), block_address, buffer.size());
            if ( hal_rc != HAL_OK )
            {
                m_error = make_error_code(m_card_handle.error());
                return {};
            }

            // Wait that read process is completed or a timeout occurs.

            if ( m_read_complete_semaphore.try_acquire_for(k_operation_timeout) == false )
            {
                return {};
            }
        }

        return buffer.size();
    }

    template <typename T>
    [[maybe_unused]] static auto write_blocks(
        const std::span<const T> buffer,
        std::uint32_t            block_address
    ) noexcept -> std::size_t
    {
        if ( buffer.empty() )
        {
            return {};
        }

        if ( is_ready() == false )
        {
            debug_assert(false);
            return {};
        }

        std::lock_guard<platform::mutex> lock{m_write_lock};

        // Write block(s) in blocking mode if called from the interrupt context (USB MCS).

        if ( arch::is_in_interrupt_handler() )
        {
            const auto hal_rc = HAL_SD_WriteBlocks(m_card_handle, std::bit_cast<const std::uint8_t*>(buffer.data()), block_address, buffer.size(), k_operation_timeout_irq.count());
            if ( hal_rc != HAL_OK )
            {
                m_error = make_error_code(m_card_handle.error());
                return {};
            }

            return buffer.size();
        }

        // Write block(s) in DMA transfer mode.

        {
            const auto hal_rc = HAL_SD_WriteBlocks_DMA(m_card_handle, std::bit_cast<const std::uint8_t*>(buffer.data()), block_address, buffer.size());
            if ( hal_rc != HAL_OK )
            {
                m_error = make_error_code(m_card_handle.error());
                return {};
            }

            // Wait that writing process is completed or a timeout occurs.

            if ( m_write_complete_semaphore.try_acquire_for(k_operation_timeout) == false )
            {
                return {};
            }
        }

        return buffer.size();
    }

    [[maybe_unused]] static auto init() noexcept -> bool
    {
        if ( auto was_initialized = m_initialized.exchange(true); was_initialized == true )
        {
            return true;
        }

        m_error = make_error_code(hal_sdmmc_error::no_error);

        hal::peripheral_type<"/chip/rcc">::enable<k_node>();

        // Initialize SDMMC peripheral pins.

        [] <typename T, T... Is> (std::integer_sequence<T, Is...>)
        {
            constexpr auto k_configuration = std::get<sdmmc_phy_config_type>(hal::peripheral_of(k_node).config);

            ([&]
            {
                constexpr auto k_pin = k_configuration.pins[Is];

                hal::peripheral_type<k_pin>::init();
            }(), ...);

        }(std::make_integer_sequence<std::size_t, sizeof(sdmmc_phy_config_type::pins) / sizeof(device_node) >{});

        // Initialize SDMMC interface.

        m_card_handle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
        m_card_handle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_ENABLE;
        m_card_handle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
        m_card_handle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
        m_card_handle.Init.ClockDiv            = 1;
        m_card_handle.Init.Transceiver         = SDMMC_TRANSCEIVER_DISABLE;

        // Initialize SD card.

        if ( HAL_SD_Init(m_card_handle) != HAL_OK )
        {
            m_error = make_error_code(m_card_handle.error());
            return m_initialized.exchange(false) == false;
        }

        // NVIC configuration for the SDMMC peripheral.

        arch::nvic::interrupt_enable<k_node>();

        // get card info.

        HAL_SD_CardInfoTypeDef card_info{};

        if ( HAL_SD_GetCardInfo(m_card_handle, std::addressof(card_info)) != HAL_OK )
        {
            m_error = make_error_code(m_card_handle.error());
            return m_initialized.exchange(false) == false;
        }

        m_block_count = card_info.LogBlockNbr;
        m_block_size  = card_info.LogBlockSize;

        return true;
    }

    [[nodiscard]] static auto block_count() noexcept { return m_block_count; }

    [[nodiscard]] static auto block_size() noexcept { return m_block_size; }

    [[nodiscard]] static auto GetError() noexcept
    {
        return m_error;
    }

    private:

    static inline std::uint32_t                        m_block_count{};
    static inline std::uint32_t                        m_block_size{};

    static inline platform::binary_semaphore           m_write_complete_semaphore{};
    static inline platform::binary_semaphore           m_read_complete_semaphore{};

    static inline constinit std::atomic<bool>          m_initialized{};

    static inline platform::mutex                      m_write_lock{};
    static inline platform::mutex                      m_read_lock{};

    static inline hal_handle<SD_HandleTypeDef, k_node> m_card_handle{};
    static inline sys::error_code                      m_error{};
};

} // namespace sys::hal
