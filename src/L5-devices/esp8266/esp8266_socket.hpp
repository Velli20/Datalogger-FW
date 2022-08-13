#pragma once

namespace sys::dev::esp8266 {

template <device_node k_instance = "/dev/wifi@0">
struct socket : public io::output_sink_interface, io::input_sink_interface
{
    static constexpr auto k_timeout = std::chrono::milliseconds(5000);

    public:

    friend struct device_type<k_instance>;

    explicit constexpr socket(std::uint8_t id) noexcept
        : m_link_id{id}
        , m_state{state_type::k_disconnected}
        {}

    auto write(
        std::span<const std::byte> data,
        std::chrono::milliseconds  timeout
    ) noexcept
    {
        if ( is_connected() == true )
        {
            return cmd::socket<k_instance>::write(data, timeout, m_link_id, true);
        }

        return false;
    }

    auto read(std::chrono::milliseconds timeout) noexcept -> std::optional<std::byte>
    {
        // auto deadline = std::chrono::high_resolution_clock::now() + timeout;

        while ( m_bytes_available.load() == 0u )
        {
            // if ( std::chrono::high_resolution_clock::now() >= deadline )
            //     return {};

            if ( m_condition_var_consumer.try_acquire_for(k_timeout) == false )
            {
                return {};
            }
        }

        auto b           = m_incoming_chunk.front();
        m_incoming_chunk = m_incoming_chunk.subspan(1);

        auto total = m_total_bytes_available.fetch_sub(1u);

        if ( m_bytes_available.fetch_sub(1u) <= 1u || total  <= 1u )
        {
            m_condition_var_waiter.release();
        }

        return b;
    }

    constexpr auto operator>>(std::span<std::byte> buffer) -> input_sink_interface& override
    {
        for ( auto& b : buffer )
        {
            *this >> b;
        }

        return *this;
    }

    constexpr auto operator>>(std::byte& b) -> input_sink_interface& override
    {
        if ( auto opt_b = read(k_timeout); opt_b.has_value() == true )
        {
            b = opt_b.value();
        }

        else
        {
            log::error() << "SOCKET operator>> timeout" << log::endl;
            b = std::byte{};
        }

        return *this;
    }

    constexpr auto operator<<(std::span<const std::byte> buffer) -> output_sink_interface& override
    {
        cmd::socket<k_instance>::write(buffer, k_timeout, m_link_id, false);

        return *this;
    }

    constexpr auto operator<<(std::byte b) -> output_sink_interface& override
    {
        cmd::socket<k_instance>::write(std::span{&b, 1}, k_timeout, m_link_id, false);

        return *this;
    }

    [[nodiscard]] auto is_connected() const
    {
        return m_state.load() == state_type::k_connected;
    }

    [[nodiscard]] auto is_connecting() const
    {
        return m_state.load() == state_type::k_connecting;
    }

    [[nodiscard]] auto is_open() const
    {
        // m_condition_var_consumer.lock();
        return m_state.load() != state_type::k_disconnected;
        // m_condition_var_consumer.release();
    }

    [[nodiscard]] auto available() const
    {
        return m_total_bytes_available.load();
    }

    [[maybe_unused]] auto close()
    {
        return device_type<k_instance>::close(*this);
    }

    private:

    [[nodiscard]] constexpr auto id() const
    {
        return m_link_id;
    }

    auto set_state(esp8266::state_type new_state)
    {
        return new_state != m_state.exchange(new_state);
    }

    auto feed(std::span<const std::byte> data) -> bool
    {
        m_incoming_chunk = data;

        m_bytes_available.store(data.size());
        m_condition_var_consumer.release();

        return m_condition_var_waiter.try_acquire_for(k_timeout);
    }

    auto notify(std::size_t bytes_waiting)
    {
        m_total_bytes_available.fetch_add(bytes_waiting);
    }

    std::span<const std::byte>       m_incoming_chunk{};
    std::uint8_t                     m_link_id{};
    std::atomic<std::size_t>         m_bytes_available{};
    std::atomic<std::size_t>         m_total_bytes_available{};
    platform::binary_semaphore       m_condition_var_consumer{};
    platform::binary_semaphore       m_condition_var_waiter{};
    std::atomic<esp8266::state_type> m_state{};
};

} // namespace sys::dev::esp8266
