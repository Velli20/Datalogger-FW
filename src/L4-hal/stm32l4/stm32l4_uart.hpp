#pragma once

namespace sys::hal {

template <device_node k_node> requires (detail::instance_of_uart_node<k_node>::value)
struct traits
{
    private:

    static constexpr auto config = std::get<uart_phy_config_type>(hal::peripheral_of(k_node).config);

    public:

    static constexpr auto clock_configuration = config.clocks;
    static constexpr auto tx_gpio             = config.tx_gpio;
    static constexpr auto rx_gpio             = config.rx_gpio;
    static constexpr auto rx_dma_channel      = config.dma_rx_channel;
    static constexpr auto rx_buffer_size      = config.rx_buffer_size;
    static constexpr auto debug               = false;
};

template <device_node k_node> requires (detail::instance_of_uart_node<k_node>::value)
struct peripheral_type<k_node> final
{
    private:

    [[nodiscard]] inline static auto get() noexcept -> UsartLayout&
    {
        return mmio::place_at<UsartLayout>(register_address_of_node<k_node>::value);
    }

    [[nodiscard]] static constexpr auto is_enabled() noexcept
    {
        return get().Control1.UsartEnable.Read() != 0ul ? true : false;
    }

    [[maybe_unused]] static constexpr auto enable(bool enable = true) noexcept
    {
        return get().Control1.UsartEnable.Write(enable ? 1ul : 0ul);
    }

    static constexpr auto reset() noexcept
    {
        get().Control1.Word.Clear();
        get().Control2.Word.Clear();
        get().Control3.Word.Clear();
    }

    struct receiver
    {
        [[nodiscard]] static constexpr auto address() noexcept
        {
            return register_address_of_node<k_node>::value + offsetof(UsartLayout, ReceiveData);
        }

        static constexpr auto enable(const bool enable = true) noexcept
        {
            get().Control1.ReceiverEnable.Write(enable ? 1ul : 0ul);
        }

        static constexpr auto enable_dma(const bool enable = true) noexcept
        {
            get().Control3.ReceiverDmaEnable.Write(enable ? 1ul : 0ul);
        }

        [[nodiscard]] static inline auto empty() noexcept -> bool
        {
            const auto rx_position = []()
            {
                return (hal::traits<k_node>::rx_buffer_size - hal::peripheral_type<hal::traits<k_node>::rx_dma_channel>::read_position()) % hal::traits<k_node>::rx_buffer_size;
            }();

            const auto current_position = m_read_buffer_position.load();

            if ( current_position == rx_position )
                return true;

            return (current_position == 0 && rx_position == hal::traits<k_node>::rx_buffer_size - 1) ? true : false;
        }
    };

    struct transmitter
    {
        static constexpr void enable(const bool enable = true) noexcept
        {
            get().Control1.TransmitterEnable.Write(enable ? 1ul : 0ul);
        }

        static constexpr auto write(const std::span<const std::byte> data) noexcept
        {
            if ( data.empty() )
            {
                return false;
            }

            constexpr auto is_tx_complete = []()
            {
                if ( get().InterruptStatus.TransmissionComplete.Read() != 0ul )
                {
                    get().InterruptFlagClear.TransmissionComplete.Set();
                    return true;
                }

                return false;
            };

            constexpr auto is_tx_empty = []()
            {
                if ( get().InterruptStatus.TxFifoNotEmpty.Read() != 0ul )
                {
                    return true;
                }

                return false;
            };

            using namespace std::chrono_literals;

            for ( const auto c : data )
            {
                if ( util::busy_wait(1000ms, is_tx_empty) == false )
                {
                    return false;
                }

                get().Transmit.DataEightBit.Write(std::to_integer<std::uint16_t>(c));
            }

            return util::busy_wait(1000ms, is_tx_complete);
        }
    };

    struct settings
    {
        static constexpr auto stop_bits(const sys::dev::stop_bits_type bits) noexcept
        {
            if ( is_enabled() )
            {
                return false;
            }

            /* Stop bits:
                Bit 00: 1 stop bit
                Bit 01: 0.5 stop bit.
                Bit 10: 2 stop bits
                Bit 11: 1.5 stop bits
            */

            switch ( bits )
            {
                case sys::dev::stop_bits_type::k_one:
                    get().Control2.stop_bits.Write(0b00ul);
                    return true;

                case sys::dev::stop_bits_type::k_two:
                    get().Control2.stop_bits.Write(0b10ul);
                    return true;
            }

            return false;
        }

        static constexpr auto parity(const sys::dev::parity_type parity) noexcept
        {
            if ( is_enabled() )
            {
                return false;
            }

            switch ( parity )
            {
                case sys::dev::parity_type::k_none:
                    [[fallthrough]];

                case sys::dev::parity_type::k_even:
                    get().Control1.ParitySelection.Clear();
                    break;

                case sys::dev::parity_type::k_odd:
                    get().Control1.ParitySelection.Set();
                    break;
            }

            return true;
        }

        static constexpr auto word_length_type(const dev::word_length_type length) noexcept
        {
            if ( is_enabled() )
            {
                return false;
            }

            /*
                This bit must be used in conjunction with bit 12 (M0) to determine
                the word length. It is set or cleared by software.

                M[1:0] = ‘00’: 1 start bit, 8 Data bits, n Stop bit
                M[1:0] = ‘01’: 1 start bit, 9 Data bits, n Stop bit
                M[1:0] = ‘10’: 1 start bit, 7 Data bits, n Stop bit
            */

            switch ( length )
            {
                case dev::word_length_type::k_eight_data_bits :
                    get().Control1.WordLengthBit0.Clear();
                    get().Control1.WordLengthBit1.Clear();
                    break;

                case dev::word_length_type::k_seven_data_bits :
                    get().Control1.WordLengthBit0.Clear();
                    get().Control1.WordLengthBit1.Set();
                    break;

                case dev::word_length_type::k_nine_data_bits :
                    get().Control1.WordLengthBit0.Set();
                    get().Control1.WordLengthBit1.Clear();
                    break;
            }

            return true;
        }

        static constexpr auto prescaler(const uart_clock_prescaler_type prescaler) noexcept
        {
            get().Prescaler.Value.Write(prescaler);
        }

        static constexpr auto baudrate(const uart_oversampling_type oversampling,
                                       const std::uint32_t    baudrate) noexcept
        {
            if ( oversampling == uart_oversampling_type::k_by_sixteen )
            {
                get().BaudRate.Value.Write(baudrate);
                get().Control1.OversamplingMode.Clear();
            }

            else if ( oversampling == uart_oversampling_type::k_by_eight )
            {
                get().BaudRate.Value.Write(baudrate);
                get().Control1.OversamplingMode.Set();
            }
        }
    };

    struct interrupt
    {
        struct enable
        {
            static constexpr auto receiver_not_empty(const bool enable = true) noexcept
            {
                get().Control1.RxFifoNotEmptyInterrupt.Write(enable ? 1ul : 0ul);
            }

            static constexpr auto transmission_complete(const bool enable = true) noexcept
            {
                get().Control1.TransmissionCompleteInterrupt.Write(enable ? 1ul : 0ul);
            }

            static constexpr auto parity_error(const bool enable = true) noexcept
            {
                get().Control1.ParityErrorInterrupt.Write(enable ? 1ul : 0ul);
            }

            static constexpr auto error(const bool enable = true) noexcept
            {
                // enable the UART Error interrupt: (Frame error, noise error, overrun error).

                get().Control3.ErrorInterruptEnable.Write(enable ? 1ul : 0ul);
            }

            static constexpr auto idle_line_detection(const bool enable = true) noexcept
            {
                get().Control1.IdleInterrupt.Write(enable ? 1ul : 0ul);
            }
        };

        [[nodiscard]] static constexpr auto is_receiver_not_empty() noexcept
        {
            return get().InterruptStatus.RxFifoNotEmpty.Read() != 0ul ? true : false;
        }

        [[nodiscard]] static constexpr auto is_idle_line_detected() noexcept
        {
            return get().InterruptStatus.IdleLineDetected.Read() != 0ul ? true : false;
        }

        static constexpr auto clear() noexcept requires(hal::traits<k_node>::debug == true)
        {
            // Parity error.

            if ( get().InterruptStatus.ParityError.Read() != 0ul )
            {
                get().InterruptFlagClear.ParityError.Set();
            }

            // Framing error.

            if ( get().InterruptStatus.FramingError.Read() != 0ul )
            {
                get().InterruptFlagClear.FramingError.Set();
            }

            // Noise detection.

            if ( get().InterruptStatus.NoiseDetection.Read() != 0ul )
            {
                get().InterruptFlagClear.NoiseDetection.Set();
            }

            // Overrun error.

            if ( get().InterruptStatus.OverrunError.Read() != 0ul )
            {
                get().InterruptFlagClear.OverrunError.Set();
            }

            get().InterruptFlagClear.All.Set();
        }

        static constexpr auto clear() noexcept
        {
            get().InterruptFlagClear.All.Set();
        }
    };

    public:

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_usart_global1 || k_type == arch::arm::vendor_irqn_type::k_uart_global4)
    static constexpr void interrupt_handler() noexcept
    {
        arch::scoped_interrupt_lock guard{};

        if ( interrupt::is_idle_line_detected() )
        {
            m_data_available_semaphore.release();
        }

        interrupt::clear();
    }

    static bool init(const dev::serial_console_config& config) noexcept
    {
        if ( auto was_initialized = m_initialized.exchange(true); was_initialized == true )
        {
            return true;
        }

        arch::scoped_interrupt_lock guard{};

        // Enable peripheral clock.

        hal::peripheral_type<"/chip/rcc">::enable<k_node>();

        // Configure TX and RX pins.

        hal::peripheral_type<hal::traits<k_node>::tx_gpio>::init();
        hal::peripheral_type<hal::traits<k_node>::rx_gpio>::init();

        // Reset peripheral registers.

        reset();

        // Configure UART.

        settings::word_length_type(config.m_word_length);
        settings::stop_bits(config.m_stop_bits);
        settings::parity(config.m_parity);

        // Interrupt configuration.

        interrupt::clear();
        interrupt::enable::receiver_not_empty();

        // Enable transmitter and receiver.

        receiver::enable();
        transmitter::enable();

        const auto frequency = hal::peripheral_type<"/chip/rcc">::get_clock_frequency<k_node>();
        if ( frequency.has_value() == false )
        {
            return m_initialized.exchange(false) == false;
        }

        // In case of oversampling by 16, the baud rate is given by the following formula:
        //
        // Tx/Rx baud = usart_ker_ckpres / USARTDIV
        //

        if constexpr ( std::get<uart_phy_config_type>(hal::peripheral_of(k_node).config).oversampling == uart_oversampling_type::k_by_sixteen )
        {
            const std::uint32_t usartdiv = (frequency.value() + (config.m_baudrate / 2ul)) / config.m_baudrate;
            settings::baudrate(uart_oversampling_type::k_by_sixteen, usartdiv);
        }

        // In case of oversampling by 8, the baud rate is given by the following formula:
        //
        // Tx/Rx baud = (2 * usart_ker_ckpres) / USARTDIV
        //

        else
        {
            const std::uint32_t usartdiv = ((frequency.value() * 2ul) + (config.m_baudrate / 2ul)) / config.m_baudrate;
            settings::baudrate(uart_oversampling_type::k_by_eight, usartdiv);
        }

        // Configure the DMA for reception process.

        // if constexpr ( rx_dma_channel != PeripheralType::kNone )
        {
            using dma = typename hal::peripheral_type<hal::traits<k_node>::rx_dma_channel>;

            dma::init();

            if constexpr ( hal::traits<k_node>::debug )
            {
                dma::interrupt::enable::transfer_error();
                dma::interrupt::enable::half_transfer_complete();
            }

            // using namespace units::literals;
            // static constexpr auto kDmaMax = 64_kB;
            // static_assert(rx_buffer_size < kDmaMax.to<std::size_t>(), "DMA buffer must be smaller than 64 kB");

            dma::interrupt::enable::transfer_complete();
            dma::start(receiver::address(), reinterpret_cast<std::uint32_t>(m_read_buffer.data()), hal::traits<k_node>::rx_buffer_size);
        }

        if constexpr ( hal::traits<k_node>::debug )
        {
            // enable the UART parity Error interrupt.

            interrupt::enable::parity_error(true);

            // enable the UART Error interrupt: (Frame error, noise error, overrun error).

            interrupt::enable::error(true);
        }

        // Enable the DMA transfer for the receiver request.

        receiver::enable_dma(true);

        // NVIC configuration for Uart.

        arch::nvic::interrupt_enable<k_node>();

        // Enable UART peripheral.

        enable(true);

        return true;
    }

    // template <std::forward_iterator Iterator>
    // requires std::convertible_to<typename std::iterator_traits<Iterator>::value_type, std::byte>

    static inline auto write(std::span<const std::byte> data) noexcept -> std::size_t
    {
        if ( data.empty() )
        {
            return {};
        }

        if ( arch::is_in_interrupt_handler() )
        {
            arch::scoped_interrupt_lock guard{};

            return transmitter::write(data) ? data.size() : 0u;
        }

        // if ( m_initialized.test(std::memory_order_relaxed) == false )
        //     return HalError::kFailedPrecondition;

        std::lock_guard<platform::mutex> lock{m_write_lock};

        return transmitter::write(data) ? data.size() : 0u;
    }

    [[maybe_unused]] static auto read(
        std::span<std::byte>      buffer,
        std::chrono::milliseconds timeout = std::chrono::milliseconds::max()
    ) noexcept -> std::size_t
    {
        if ( buffer.empty() )
        {
            return {};
        }

        std::lock_guard<platform::mutex> lock{m_read_lock};

        if ( timeout == std::chrono::milliseconds::zero() && receiver::empty() == true )
        {
            return {};
        }

        std::size_t bytes_in{};

        auto next = [timeout](std::byte& c)
        {
            std::chrono::milliseconds begin{};

            while ( receiver::empty() == true )
            {
                if ( auto rc = m_data_available_semaphore.try_acquire_for(std::chrono::milliseconds(40)); rc == false )
                {
                    begin += std::chrono::milliseconds(40);
                    if ( begin > timeout )
                    {
                        return false;
                    }

                    continue;
                    /* return false; */
                }
            }

            auto index = m_read_buffer_position.fetch_add(1ul);
            if ( index >= hal::traits<k_node>::rx_buffer_size )
            {
                m_read_buffer_position.store(0ul);
            }

            c = m_read_buffer.data()[index];

            // if constexpr ( k_node == "/chip/uart@4")
            // {
            //     if ( c == std::byte{'\r'} )
            //     {
            //         constexpr const auto k_esc = std::string_view{"\\r"};
            //         hal::peripheral_type<"/chip/usart@1">::Write(util::as_byte_data(k_esc));
            //     }

            //     if ( c == std::byte{'\n'} )
            //     {
            //         constexpr const auto k_esc = std::string_view{"\\n"};
            //         hal::peripheral_type<"/chip/usart@1">::Write(util::as_byte_data(k_esc));
            //     }


            //     hal::peripheral_type<"/chip/usart@1">::Write(std::span<std::byte>{&c, 1});
            // }

            return true;
        };

        for ( auto& c : buffer )
        {
            if ( next(c) == false )
            {
                break;
            }

            bytes_in += 1;
        }

        return bytes_in;
    }

    [[nodiscard]] static auto is_ready() noexcept -> bool
    {
        return m_initialized.load();
    }

    static inline constinit std::atomic<std::size_t>   m_read_buffer_position{};
    static inline constinit std::atomic<bool>          m_initialized{};

    static inline platform::mutex                      m_write_lock{};
    static inline platform::mutex                      m_read_lock{};
    static inline platform::binary_semaphore           m_data_available_semaphore{};

    static inline std::array<std::byte, hal::traits<k_node>::rx_buffer_size> m_read_buffer{};
};

} // namespace sys::hal
