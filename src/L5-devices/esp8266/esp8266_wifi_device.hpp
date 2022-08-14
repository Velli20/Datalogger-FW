#pragma once

namespace sys::dev {

template <device_node k_instance>
struct device_type<k_instance, typename std::enable_if<is_wifi_device<k_instance>::value>::type>
{
    private:

    using server_type   = const http::router_interface;
    using response_type = dev::response<esp8266::atcode_type>;

    friend struct esp8266::socket<k_instance>;
    friend struct esp8266::lock<k_instance>;

    friend struct esp8266::cmd::access_point<k_instance>;
    friend struct esp8266::cmd::sntp<k_instance>;
    friend struct esp8266::cmd::power<k_instance>;
    friend struct esp8266::cmd::settings<k_instance>;
    friend struct esp8266::cmd::query<k_instance>;
    friend struct esp8266::cmd::socket<k_instance>;
    friend struct esp8266::cmd::dns<k_instance>;

    static constexpr auto make_sockets()
    {
        return [] <typename T, T... Is> (std::integer_sequence<T, Is...>)
        {
            return std::array<esp8266::socket<k_instance>, sizeof...(Is)> { esp8266::socket<k_instance>{Is}... };
        }(std::make_integer_sequence<std::uint8_t, 4>{});
    }

    constexpr device_type() = delete;

    static constexpr auto k_configuration = std::get<wifi_config_type>(device_tree::get(k_instance));
    static constexpr auto k_uart          = k_configuration.uart;
    static constexpr auto k_debug         = true;
    static constexpr auto k_socket_max    = std::uint8_t{5};

    static inline auto m_unsolicited_responses = platform::pmr::vector<esp8266::unsolicited_response>       { platform::memory::default_resource() };
    static inline auto m_routers               = platform::pmr::vector<std::reference_wrapper<server_type>> { platform::memory::default_resource() };
    static inline auto m_error                 = sys::error_code{};
    static inline auto m_lock                  = platform::mutex{};
    static inline auto m_sockets               = make_sockets();
    static inline auto m_server_mode           = false;

    static auto next() -> std::optional<response_type>
    {
        static constexpr auto k_timeout = std::chrono::milliseconds(0);

        utils::ringbuffer<char, 128> rb{};

        while ( true )
        {
            std::byte data{};

            if ( device_type<k_uart>::read(std::span{std::addressof(data), 1}, k_timeout) == 0u )
            {
                m_error = make_error_code(esp8266_error_type::k_at_timeout);
                return {};
            }

            rb.push_back(std::to_integer<char>(data));

            for ( auto& resp : esp8266::response_table::any::value )
            {
                if ( resp.equals(rb) )
                {
                    return resp;
                }
            }
        }
    }

    static auto parse(std::integral_constant<esp8266::atcode_type, esp8266::atcode_type::k_incoming_packet_data>) -> sys::expected<std::pair<std::uint32_t, std::uint32_t>, esp8266_error_type>
    {
        using namespace std::chrono_literals;

        auto parser = esp8266::parse::ipd{};

        // Read and parse incoming +IPD response.

        while ( true )
        {
            std::byte data{};

            if ( device_type<k_uart>::read(std::span{std::addressof(data), 1}, 6000ms) != 1 )
            {
                return {};
            }

            if ( std::invoke(parser, std::to_integer<char>(data)) == false )
            {
                break;
            }
        }

        // Check that parsed connection id and incoming data count are valid.

        if ( parser.connection().has_value() == false || parser.count().has_value() == false )
        {
            return {};
        }

        return std::make_pair(parser.connection().value(), parser.count().value());
    }

    static auto parse(...)
    {
        return tl::make_unexpected(esp8266_error_type::k_invalid_argument);
    }

    template <typename T>
    static auto wait(
        std::span<const dev::response<T>> responses,
        std::chrono::milliseconds         timeout = std::chrono::milliseconds(5000)
    ) -> tl::expected<T, esp8266_error_type>
    {
        utils::ringbuffer<char, 128> rb{};

        while ( true )
        {
            std::byte data{};

            if ( device_type<k_uart>::read(std::span{std::addressof(data), 1}, timeout) == 0u )
            {
                return tl::make_unexpected(esp8266_error_type::k_at_timeout);
            }

            rb.push_back(std::to_integer<char>(data));

            for ( auto& resp : esp8266::response_table::any::value )
            {
                if ( resp.equals(rb) == false )
                {
                    continue;
                }

                if ( responses.empty() == false  && std::find(std::begin(responses), std::end(responses), resp) != std::end(responses) )
                {
                    return resp.code();
                }

                if ( resp.code() == esp8266::atcode_type::k_incoming_packet_data )
                {
                    static constexpr auto k_ipd = std::integral_constant<esp8266::atcode_type, esp8266::atcode_type::k_incoming_packet_data>{};

                    auto rc = parse(k_ipd).and_then([code = resp.code()](auto v) -> sys::expected<void, esp8266_error_type>
                    {
                        auto [connection, bytes_available] = v;

                        m_unsolicited_responses.emplace_back(code, connection, bytes_available);

                        return {};
                    });

                    if ( rc.has_value() == false )
                    {
                        log::error() << "Failed to handle +IPD" << log::endl;
                    }
                }

                else if ( resp.code() == esp8266::atcode_type::k_network_disconnected || resp.code() == esp8266::atcode_type::k_network_connected )
                {
                    if ( auto opt = sys::algorithm::string_to_integral<std::uint32_t>(resp.text(), 10); opt.has_value() )
                    {
                        m_unsolicited_responses.emplace_back(resp.code(), opt.value());
                    }

                    else
                    {
                        log::error() << "Failed to handle CLOSED/CONNECT" << log::endl;
                    }
                }

                else
                {
                    m_unsolicited_responses.emplace_back(resp.code());
                }

                break;
            }
        }
    }

    static auto write(std::span<const std::byte> data) -> bool
    {
        if ( data.empty() )
        {
            return true;
        }

        if ( auto byte_written = device_type<k_uart>::write(data); byte_written != data.size() )
        {
            m_error = make_error_code(esp8266_error_type::k_at_write);
            return false;
        }

        return true;
    }

    static auto lock()
    {
        return m_lock.lock();
    }

    static auto unlock()
    {
        return m_lock.unlock();
    }

    static auto read(
        std::span<std::byte>      buffer,
        std::chrono::milliseconds timeout
    ) -> bool
    {
        if ( device_type<k_uart>::read(buffer, timeout) == 0u )
        {
            m_error = make_error_code(esp8266_error_type::k_at_timeout);
            return false;
        }

        return true;
    }

    static auto read(
        concepts::socket_callback auto&& callback,
        std::chrono::milliseconds        timeout,
        std::size_t                      bytes_to_read = std::numeric_limits<std::size_t>::max()
    ) -> bool
    {
        for ( std::size_t i{}; i != bytes_to_read; ++i )
        {
            std::byte data{};

            if ( device_type<k_uart>::read(std::span{std::addressof(data), 1}, timeout) == 0u )
            {
                m_error = make_error_code(esp8266_error_type::k_at_timeout);
                return false;
            }

            if ( auto c = std::to_integer<char>(data); std::invoke(std::forward<decltype(callback)>(callback), c) == false )
            {
                return true;
            }
        }

        return true;
    }

    template <typename T>
    static T execute_command(
        std::span<const dev::response<T>> responses,
        std::span<const std::byte>        data,
        std::chrono::milliseconds         timeout = std::chrono::milliseconds(5000)
    )
    {
        if ( data.empty() == false && write(data) != true )
        {
            return {};
        }

        if ( auto result = wait(responses, timeout); result.has_value() == true )
        {
            return result.value();
        }

        m_error = make_error_code(esp8266_error_type::k_at_timeout);
        return {};
    }

    template <typename T>
    static decltype(auto) execute_command(
        std::span<const dev::response<T>> responses,
        std::span<const char>             data,
        std::chrono::milliseconds         timeout = std::chrono::milliseconds(5000)
    )
    {
        return execute_command<T>(responses, std::as_bytes(data), timeout);
    }

    public:

    static bool get_sntp_time(std::tm& t)
    {
        return esp8266::cmd::sntp<k_instance>::now(t);
    }

    static bool list_access_points(concepts::access_point_found_callback auto callback)
    {
        return esp8266::cmd::access_point<k_instance>::list(std::forward<decltype(callback)>(callback));
    }

    static bool list_clients(concepts::access_point_client_info_callback auto callback)
    {
        return esp8266::cmd::access_point<k_instance>::clients(std::forward<decltype(callback)>(callback));
    }

    static bool create_access_point(
        std::string_view         ssid,
        esp8266::encryption_type encryption_type,
        std::string_view         password        = std::string_view{},
        bool                     hidden          = false,
        std::uint8_t             max_connections = 10
    ) noexcept
    {
        m_server_mode = true;

        return esp8266::cmd::access_point<k_instance>::create(ssid, encryption_type, password, hidden, max_connections);
    }

    static auto get_ip_address() noexcept -> std::optional<esp8266::ip_address_type>
    {
        esp8266::ip_address_type ip{};

        if ( esp8266::cmd::query<k_instance>::access_point_ip(ip) == true )
        {
            return ip;
        }

        return {};

    }

    static bool enter_deep_sleep(std::chrono::milliseconds sleep_time) noexcept
    {
        if ( esp8266::cmd::power<k_instance>::set_mode(esp8266::sleep_mode_type::k_modem) == true )
        {
            return esp8266::cmd::power<k_instance>::deep_sleep(sleep_time);
        }

        return false;
    }

    static bool disconnect_from_access_point() noexcept
    {
        return esp8266::cmd::access_point<k_instance>::disconnect();
    }

    static bool connect_to_access_point(
        std::string_view m_ssid,
        std::string_view password
    ) noexcept
    {
        m_server_mode = false;

        return esp8266::cmd::access_point<k_instance>::connect(m_ssid, password);
    }

    static auto start(
        std::uint16_t            port_number,
        esp8266::connection_type type
    ) noexcept
    {
        return esp8266::cmd::socket<k_instance>::start(port_number, type);
    }

    static auto write(const http::request_view& req) noexcept -> http::message
    {
        using namespace std::chrono_literals;

        if ( req.host().empty() )
        {
            return {};
        }

        auto socket_opt = device_type<k_instance>::open(req.host(), req.port(), dev::esp8266::connection_type::k_tcp, 5s);
        if ( socket_opt.has_value() == false )
        {
            log::info() << "Failed to establish TCP connection to:" << req.host() << log::endl;
            return {};
        }

        auto socket = socket_opt.value();
        req.write(socket.get());

        auto http_parser = http::parse::header{platform::memory::default_resource()};

        do
        {
            if ( auto next_byte = socket.get().read(5000ms); next_byte.has_value() )
            {
                auto c = std::to_integer<char>(next_byte.value());
                if ( http_parser(c) == false )
                {
                    break;
                }

                continue;
            }

            log::info() << "Timeout" << log::endl;
        }
        while ( socket.get().is_connected() );

        auto response_opt = http_parser.finalize();
        if ( response_opt.has_value() == false )
        {
            log::error() << "Invalid HTTP message" << log::endl;
            return {};
        }

        auto response = std::move(response_opt.value());
        log::info() << "Response content length: " << response.content_length() << log::endl;

        // Get body content length.

        if ( response.content_length() < 1 )
        {
            socket.get().close();
            return response;
        }

        response.set_body(socket.get(), response.content_length());

        // if ( socket.get().close() != true )
        // {
        //     log::info() << "Failed to close socket" << log::endl;
        // }

        return response;
    }

    static auto open(
        std::string_view          host,
        std::uint16_t             port,
        esp8266::connection_type  type,
        std::chrono::milliseconds timeout
    ) noexcept -> std::optional<std::reference_wrapper<esp8266::socket<k_instance>>>
    {
        using namespace std::chrono_literals;

        // Find free socket.

        auto socket_opt = []() -> std::optional<std::reference_wrapper<esp8266::socket<k_instance>>>
        {
            for ( auto& s : m_sockets )
            {
                if ( s.is_open() == false )
                {
                    return std::reference_wrapper<esp8266::socket<k_instance>>{s};
                }
            }

            return {};
        }();

        if ( socket_opt.has_value() == false )
        {
            m_error = make_error_code(esp8266_error_type::k_socket_open);
            return {};
        }

        auto& socket = socket_opt.value().get();
        socket.set_state(esp8266::state_type::k_connecting);

        if ( esp8266::cmd::socket<k_instance>::open(host, port, socket.id(), type, timeout) == false )
        {
            m_error = make_error_code(esp8266_error_type::k_socket_open);
            socket.set_state(esp8266::state_type::k_disconnected);
            return {};
        }

        if ( util::busy_wait(timeout, [&]() { return socket.is_connected(); }) == true )
        {
            return socket_opt;
        }

        return {};
    }

    static bool close(auto& socket) noexcept
    {
        if ( socket.is_open() == false )
        {
            return false;
        }

        if ( esp8266::cmd::socket<k_instance>::close(socket.id()) == false )
        {
            m_error = make_error_code(esp8266_error_type::k_socket_open);
            return false;
        }

        socket.set_state(esp8266::state_type::k_disconnected);

        return true;
    }

    static bool try_serve() noexcept
    {
        using namespace std::chrono_literals;

        // std::lock_guard<platform::mutex> lock{m_lock};

        if ( m_server_mode == false )
        {
            return false;
        }

        for ( auto& s : m_sockets )
        {
            if ( s.available() < 1 || s.is_connected() == false )
            {
                continue;
            }

            auto http_parser = http::parse::header{platform::memory::default_resource()};
            do
            {
                if ( auto next_byte = s.read(5000ms); next_byte.has_value() )
                {
                    auto c = std::to_integer<char>(next_byte.value());

                    if ( http_parser(c) == false )
                    {
                        break;
                    }

                    continue;
                }

                log::error() << "[TRY SERVE]" << "Timeout" << log::endl;
                return {};
            }
            while ( true );

            // if ( s.read(http_parser, 5s) == false )
            //     continue;

            auto message_opt = http_parser.finalize();
            if ( message_opt.has_value() == false )
            {
                log::error() << "[TRY SERVE]" << "Invalid HTTP message" << log::endl;
                continue;
            }

            auto http_message = std::move(message_opt.value());
            if ( http_message.content_length() > 0 )
            {
                http_message.set_body(s, http_message.content_length());
            }

            // Call handler and get response.

            auto&& server_response = [&]() -> http::router_response_type
            {
                for ( auto& server : m_routers )
                {
                    if ( auto resp = std::invoke(server.get(), http_message); resp.has_value() )
                    {
                        return std::move(*resp);
                    }
                }

                constexpr auto k_not_found = std::string_view{"<!doctype html><html lang=en><title>Error 404: Resource not found</title>"};

                return http::response_view{http::status_code_type::k_not_found, k_not_found};
            }();

            // Write response.

            if ( std::holds_alternative<http::response_view>(server_response) )
            {
                std::get<http::response_view>(server_response).write(s);
            }

            else if ( std::holds_alternative<http::response>(server_response) )
            {
                std::get<http::response>(server_response).write(s);
            }
        }

        return false;
    }

    static bool feed_socket(auto& socket) noexcept
    {
        auto bytes_available = socket.available();

        log::info() << "feed_socket id: " << socket.id() << " bytes: " << bytes_available << log::endl;
        if ( bytes_available < 1u )
        {
            return false;
        }

        if ( socket.is_connected() == false )
        {
            log::error() << "Socket " << socket.id() << " not connected" << log::endl;
        }

        // Read incoming data chunk by chunk and then wait until the socket has consumed the
        // incoming chunk.

        std::array<std::byte, 128> buffer;

        while ( bytes_available > 0u )
        {
            auto to_read = std::min(buffer.size(), static_cast<std::size_t>(bytes_available));
            auto chunk   = std::span{buffer.data(), to_read};

            static constexpr auto k_timeout = std::chrono::milliseconds(2000);

            if ( esp8266::cmd::socket<k_instance>::read(chunk, k_timeout, socket.id()) != true )
            {
                log::error() << "Socket " << socket.id() << " data read tiemout" << log::endl;
                return false;
            }

            if ( socket.feed(chunk) == false )
            {
                log::error() << "Socket " << socket.id() << " unhandled data" << log::endl;
            }

            bytes_available -= to_read;
        }

        return true;
    }

    static bool handle_incoming_data_packet(
        std::uint32_t connection,
        std::uint32_t bytes_available
    ) noexcept
    {
        using namespace std::chrono_literals;

        log::info() << "Got " << bytes_available << " for socket " << connection << log::endl;

        // Find target socket.

        for ( auto& s : m_sockets )
        {
            if ( s.id() != connection )
            {
                continue;
            }

            if ( s.is_connected() == false )
            {
                log::error() << "Socket " << connection << " not connected" << log::endl;
            }

            s.notify(bytes_available);

            return true;
        }

        log::error() << "Socket " << connection << " unhandled data" << log::endl;

        return false;
    }

    static auto handle(const esp8266::unsolicited_response& c) noexcept
    {
        if ( c.code() == esp8266::atcode_type::k_network_disconnected || c.code() == esp8266::atcode_type::k_network_connected )
        {
            if ( c.data(0) >= std::numeric_limits<std::uint8_t>::max() )
            {
                log::error() << "Socket number overflow" << log::endl;
                return;
            }

            auto id = static_cast<std::uint8_t>(c.data(0));

            if ( c.code() == esp8266::atcode_type::k_network_disconnected )
            {
                log::debug() << "Socket closen " << id << log::endl;
            }

            else
            {
                log::debug() << "Socket open " << id << log::endl;
            }

            for ( auto& s : m_sockets )
            {
                if ( s.id() == id )
                {
                    s.set_state(c.code() == esp8266::atcode_type::k_network_connected ? esp8266::state_type::k_connected : esp8266::state_type::k_disconnected);
                    break;
                }
            }
        }

        else if ( c.code() == esp8266::atcode_type::k_incoming_packet_data )
        {
            handle_incoming_data_packet(c.data(0), c.data(1));
        }

        else
        {
            log::info() << "Unhandled response " /* << c.text() */ << static_cast<std::uint32_t>(c.code()) << log::endl;
        }
    }

    static auto handle_unsolicited_responses() noexcept
    {
        if ( m_unsolicited_responses.empty() == true )
        {
            return true;
        }

        auto copy = platform::pmr::vector<esp8266::unsolicited_response> { m_unsolicited_responses, platform::memory::default_resource() };

        m_unsolicited_responses.clear();

        for ( auto it = copy.cbegin(), end = copy.cend(); it != end; ++it )
        {
            handle(*it);
        }

        return true;
    }


    static auto init() noexcept -> bool
    {
        m_error = make_error_code(esp8266_error_type::k_no_error);

        m_unsolicited_responses.reserve(10);
        m_routers.reserve(4);
        // m_callbacks.reserve(3);

        device_type<k_uart>::init();

        static constexpr auto k_reset_pin = k_configuration.reset;
        static constexpr auto k_gpio0_pin = k_configuration.cs;
        //static constexpr auto k_gpio0_pin = k_configuration.download;

        hal::peripheral_type<k_gpio0_pin>::init(); // Flash boot: floating

        hal::peripheral_type<k_reset_pin>::init();
        hal::peripheral_type<k_reset_pin>::set();

        this_thread::sleep_for(std::chrono::milliseconds(1500));

        // TODO: Use state machine.

        auto rc = wait(esp8266::response_table::make<esp8266::atcode_type::k_ready>::value)
        .and_then([](auto n)
        {
            // Echo off.

            return esp8266::cmd::settings<k_instance>::echo(false);
        })
        .and_then([]()
        {
            // Multiple connections on.

            return esp8266::cmd::settings<k_instance>::multiple_connections(true);
        })
        .and_then([]()
        {
            // Show remote address and port on +IPD.

            return esp8266::cmd::settings<k_instance>::ipd_info(true);
        })
        .and_then([]()
        {
            // Station mode:

            return esp8266::cmd::settings<k_instance>::station_mode(esp8266::station_mode::kSoftApAndStation);
        })
        .and_then([]()
        {
            // Passive receive mode.

            return esp8266::cmd::settings<k_instance>::enable_passive_receive_mode(true);
        })
        .and_then([]() -> tl::expected<void, esp8266_error_type>
        {
            if ( esp8266::cmd::query<k_instance>::version() == false )
                return tl::make_unexpected(esp8266_error_type::k_multiple_connections);

            return {};
        });

        if ( rc.has_value() == false )
        {
            log::error() << "Failed to set DNS" << log::endl;
        }

        constexpr auto k_dns1 = std::string_view{"8.8.8.8"};
        constexpr auto k_dns2 = std::string_view{"8.8.4.4"};

        if ( esp8266::cmd::dns<k_instance>::set(k_dns1, k_dns2) == false )
        {
            log::error() << "Failed to set DNS" << log::endl;
        }

        auto primary   = platform::pmr::string{platform::memory::default_resource()};
        auto secondary = platform::pmr::string{platform::memory::default_resource()};

        if ( esp8266::cmd::dns<k_instance>::get(primary, secondary) == false )
        {
            log::error() << "Failed to read DNS" << log::endl;
        }

        log::info() << "Primary DNS:   " << primary   << log::endl;
        log::info() << "Secondary DNS: " << secondary << log::endl;

        // SNTP server.

        if constexpr ( k_configuration.sntp_server.empty() == false )
        {
            return esp8266::cmd::sntp<k_instance>::enable(3, k_configuration.sntp_server);
        }

        return true;
    }

    static auto run() noexcept -> bool
    {
        std::lock_guard<platform::mutex> lock{m_lock};

        handle_unsolicited_responses();
        wait(std::span<const dev::response<esp8266::atcode_type>>{}, std::chrono::milliseconds(0));
        handle_unsolicited_responses();

        for ( auto& s : m_sockets )
        {
            if ( /* s.is_connected() && */ s.available() > 0u )
            {
                feed_socket(s);
            }
        }

        return true;
    }

    static auto listen(server_type& server) noexcept
    {
        std::lock_guard<platform::mutex> lock{m_lock};

        m_routers.push_back(std::reference_wrapper{server});
    }
};

} // namespace sys::dev::esp8266
