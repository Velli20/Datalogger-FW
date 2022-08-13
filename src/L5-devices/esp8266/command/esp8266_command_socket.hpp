#pragma once

namespace sys::dev::esp8266::cmd {

template <device_node k_instance = "/dev/wifi@0">
struct socket
{
    static bool open(
        std::string_view          host,
        std::uint16_t             port,
        std::uint16_t             id,
        esp8266::connection_type  type,
        std::chrono::milliseconds timeout
    )
    {
        // Get a connection type string for an ESP8266 AT-command.

        const auto type_string = [](esp8266::connection_type p) -> std::string_view
        {
            using namespace std::string_view_literals;

            constexpr std::tuple<esp8266::connection_type, std::string_view> k_types[] =
            {
                { esp8266::connection_type::k_tcp, "TCP"sv, },
                { esp8266::connection_type::k_udp, "UDP"sv, },
                { esp8266::connection_type::k_ssl, "SSL"sv, },
            };

            for ( auto [t, s] : k_types )
            {
                if ( t == p )
                {
                    return s;
                }
            }

            return {};
        }(type);

        std::array<char, 256> buffer;

        // Construct an AT-command.

        auto command = [&]() -> std::span<char>
        {

            // TODO: string_view is not quaranteed to be null terminated.

            std::array<char, 128> tmp_host;
            assert(host.size() < tmp_host.size());

            std::copy(std::cbegin(host), std::cend(host), std::begin(tmp_host));
            tmp_host[host.size()] = '\0';

            static constexpr auto k_format = R"(AT+CIPSTART=%d,"%s","%s",%d)" "\r\n";

            auto length = std::snprintf(buffer.data(), buffer.size(), k_format, id, type_string.data(), tmp_host.data(), port);
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};

        }();

        log::info() << "CMD:" << buffer.data() << log::endl;

        if ( command.empty() )
        {
            return false;
        }

        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail,
            esp8266::atcode_type::k_network_already_connected
        >::value;

        esp8266::lock<k_instance> lock{};

        auto rc = device_type<k_instance>::execute_command(k_expected, command, timeout);
        if ( rc != esp8266::atcode_type::k_ok && rc != esp8266::atcode_type::k_network_already_connected )
        {
            // m_error = make_error_code(esp8266::errc::socket_open);
            return false;
        }

        return true;
    }

    static bool close(std::uint16_t id)
    {
        /// CIPCLOSE:
        ///
        /// AT+CIPCLOSE=<link ID>
        ///
        /// **<link ID>**: ID of the connection that you want to close. If you set it to 5, all connections will be closed.

        std::array<char, 128> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CIPCLOSE=%d)" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, id);
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};
        }();

        if ( command.empty() )
        {
            return false;
        }

        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        return device_type<k_instance>::execute_command(k_expected, command) == esp8266::atcode_type::k_ok;
    }

    static auto start(std::uint16_t            port_number,
                      esp8266::connection_type type)
    {
        std::array<char, 128> buffer;

        // Construct an AT-command.

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CIPSERVER=1,%d)" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, port_number);
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};
        }();

        if ( command.empty() )
        {
            return false;
        }

        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        using namespace std::chrono_literals;

        return device_type<k_instance>::execute_command(k_expected, command, 5000ms) == esp8266::atcode_type::k_ok;
    }

    static auto set_max_connections(std::uint16_t max)
    {
        std::array<char, 128> buffer;

        // Construct an AT-command.

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(CIPSERVERMAXCONN=%d)" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, max);
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};
        }();

        if ( command.empty() )
        {
            return false;
        }

        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        return device_type<k_instance>::execute_command(k_expected, command) == esp8266::atcode_type::k_ok;
    }

    static auto receive_length()
    {
        using namespace std::chrono_literals;

        esp8266::lock<k_instance> lock{};

        {
            static constexpr auto k_command  = util::make_byte_data("AT+CIPRECVLEN?\r\n");
            static constexpr auto k_expected = esp8266::response_table::make<
                esp8266::atcode_type::k_query_available_data,
                esp8266::atcode_type::k_error,
                esp8266::atcode_type::k_fail
            >::value;

            if ( device_type<k_instance>::execute_command(k_expected, k_command) != esp8266::atcode_type::k_query_available_data )
            {
                return false;
            }
        }

        struct
        {
            constexpr auto operator()(char c)
            {
                if ( c == '\r' )
                {
                    return false;
                }

                return true;
            }

        } parser{};

        if ( device_type<k_instance>::read(parser, 1000ms) != true )
        {
            return false;
        }

        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        return device_type<k_instance>::wait(k_expected, 2000ms) == esp8266::atcode_type::k_ok;
    }


    static auto read(
        std::span<std::byte>      data,
        std::chrono::milliseconds timeout,
        std::uint8_t              link_id
    )

    {
        std::array<char, 256> buffer;

        if ( data.empty() || data.size() > 2048u )
        {
            return false;
        }

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = "AT+CIPRECVDATA=%d,%d\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, link_id, data.size());
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};
        }();

        if ( command.empty() )
        {
            return false;
        }

        using namespace std::chrono_literals;

        // esp8266::lock<k_instance> lock{};

        {
            static constexpr auto k_expected = esp8266::response_table::make<
                esp8266::atcode_type::k_receive_data,
                esp8266::atcode_type::k_error,
                esp8266::atcode_type::k_fail
            >::value;

            if ( device_type<k_instance>::execute_command(k_expected, command) != esp8266::atcode_type::k_receive_data )
            {
                return false;
            }
        }

        struct socket_reader_type
        {
            constexpr socket_reader_type(std::span<std::byte> b)
                : m_buffer{b}
                , m_received{}
                {}

            [[maybe_unused]] constexpr auto operator()(char c)
            {
                if ( m_in_data && c == '\r' && m_received == m_buffer.size() )
                {
                    return false;
                }

                if ( not m_in_data && c == ':' && m_received == 0u )
                {
                    m_in_data = true;
                    return true;
                }

                else if ( m_in_data && m_received != m_buffer.size() )
                {
                    m_buffer.data()[m_received] = std::byte{c};
                    m_received++;
                }

                return true;
            }

            std::span<std::byte> m_buffer{};
            std::size_t          m_received{};
            bool                 m_in_data{};
        };

        if ( auto parser = socket_reader_type{data}; device_type<k_instance>::read(parser, 1000ms) != true )
        {
            return false;
        }

        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error
        >::value;

        return device_type<k_instance>::wait(k_expected, 2000ms) == esp8266::atcode_type::k_ok;
    }

    static auto write(
        std::span<const std::byte> data,
        std::chrono::milliseconds  timeout,
        std::uint8_t               link_id,
        bool                       acquire_lock
    )
    {
        std::array<char, 256> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = "AT+CIPSENDEX=%d,%d\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, link_id, data.size());
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};
        }();

        if ( command.empty() )
        {
            return false;
        }

        if ( acquire_lock == true )
        {
            esp8266::lock<k_instance> lock{};
        }

        {
            static constexpr auto k_expected = esp8266::response_table::make<
                esp8266::atcode_type::k_ok,
                esp8266::atcode_type::k_error
            >::value;

            if ( device_type<k_instance>::execute_command(k_expected, command, timeout) != esp8266::atcode_type::k_ok )
            {
                return false;
            }
        }

        {
            static constexpr auto k_expected = esp8266::response_table::make<esp8266::atcode_type::k_ready_to_write>::value;

            auto response = device_type<k_instance>::wait(k_expected, timeout);
            if ( response.has_value() == false || response.value() != esp8266::atcode_type::k_ready_to_write )
            {
                return false;
            }
        }

        {
            static constexpr auto k_expected = esp8266::response_table::make<
                esp8266::atcode_type::k_send_ok,
                esp8266::atcode_type::k_send_fail
            >::value;

            if ( device_type<k_instance>::execute_command(k_expected, data, timeout) != esp8266::atcode_type::k_send_ok )
            {
                return false;
            }
        }

        return true;
    }
};

} // namespace sys::dev::esp8266::cmd
