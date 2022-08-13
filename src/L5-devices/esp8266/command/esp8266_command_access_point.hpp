#pragma once

namespace sys::dev::esp8266::cmd {

template <device_node k_instance = "/dev/wifi@0">
struct access_point
{
    static bool clients(concepts::access_point_client_info_callback auto callback)
    {
        using namespace std::chrono_literals;

        static constexpr auto k_command  = util::make_byte_data("AT+CWLIF\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail,
            esp8266::atcode_type::k_query_station_clients
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( auto rc = device_type<k_instance>::write(std::span{k_command}); rc != true )
        {
            // m_error = make_error_code(esp8266::errc::at_command_send);
            return false;
        }

        esp8266::ip_address_type  ip{};
        esp8266::mac_address mac{};

        while ( true )
        {
            if ( auto response = device_type<k_instance>::wait(k_expected, 2000ms); response != esp8266::atcode_type::k_query_station_clients )
            {
                return response == esp8266::atcode_type::k_ok ? true : false;
            }

            if ( auto parser = esp8266::parse::connected_client{ip, mac}; device_type<k_instance>::read(parser, 10000ms) != true )
            {
                return false;
            }

            std::invoke(callback, ip, mac);
        }
    }

    static bool list(concepts::access_point_found_callback auto callback)
    {
        using namespace std::chrono_literals;

        static constexpr auto k_command  = util::make_byte_data("AT+CWLAP\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail,
            esp8266::atcode_type::k_quaery_access_point
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( auto rc = device_type<k_instance>::write(std::span{k_command}); rc != true )
        {
            // m_error = make_error_code(esp8266::errc::at_command_send);
            return false;
        }

        esp8266::ap_info_type ap{};

        while ( true )
        {
            if ( auto response = device_type<k_instance>::wait(k_expected, 30000ms); response != esp8266::atcode_type::k_quaery_access_point )
            {
                return response == esp8266::atcode_type::k_ok ? true : false;
            }

            if ( auto parser = esp8266::parse::access_point{ap}; device_type<k_instance>::read(parser, 10000ms) != true )
            {
                return false;
            }

            std::invoke(callback, ap);
        }
    }

    static bool create(
        std::string_view        ssid,
        esp8266::encryption_type     encryption_type,
        std::string_view        password        = std::string_view{},
        bool                    hidden          = false,
        std::uint8_t            max_connections = 10
    ) noexcept
    {
        if ( encryption_type != esp8266::encryption_type::k_open && password.size() < 8 )
        {
            log::error() << "Invalid password length (min 8 chars)." << log::endl;
            return false;
        }

        std::array<char, 128> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format= R"(AT+CWSAP="%s","%s",5,%ld)" "\r\n";

            const auto length = std::snprintf(
                buffer.data(),
                buffer.size(),
                k_format,
                ssid.data(),
                password.empty() ? "" : password.data(),
                static_cast<std::uint32_t>(encryption_type)
            );

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

        if ( device_type<k_instance>::execute_command(k_expected, command) != esp8266::atcode_type::k_ok )
        {
            // m_error = make_error_code(esp8266::errc::create_access_point);
            return false;
        }

        return true;
    }

    static bool connect(std::string, std::string) = delete;

    static bool connect(
        std::string_view ssid,
        std::string_view password
    ) noexcept
    {
        using namespace std::chrono_literals;

        std::array<char, 128> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CWJAP_CUR="%s","%s")" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, ssid.data(), password.data());
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

        if ( device_type<k_instance>::execute_command(k_expected, command, 10000ms) != esp8266::atcode_type::k_ok )
        {
            // m_error = make_error_code(esp8266::errc::connect_to_access_point);
            return false;
        }

        return true;
    }

    static bool disconnect()
    {
        static constexpr auto k_command  = util::make_byte_data("AT+CWQAP\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( device_type<k_instance>::execute_command(k_expected, std::span{k_command}) != esp8266::atcode_type::k_ok )
        {
            // m_error = make_error_code(esp8266::errc::disconnect_from_access_point);
            return false;
        }

        return true;
    }

    static bool set_reconnect_configuration(
        std::chrono::seconds interval,
        std::uint16_t        repeat_count = 1
    ) noexcept
    {
        using namespace std::chrono_literals;

        std::array<char, 128> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CWRECONNCFG=%lld,%d)" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, interval.count(), repeat_count);
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

        return device_type<k_instance>::execute_command(k_expected, command, 10000ms) == esp8266::atcode_type::k_ok;
    }
};

} // namespace sys::dev::esp8266::cmd
