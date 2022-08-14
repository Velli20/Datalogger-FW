#pragma once

namespace sys::dev::esp8266::cmd {

template <device_node k_instance = "/dev/wifi@0">
struct settings
{
    static auto enable_passive_receive_mode(bool enable = true) noexcept -> tl::expected<void, esp8266_error_type>
    {
        static constexpr auto k_enable   = util::make_byte_data("AT+CIPRECVMODE=1\r\n");
        static constexpr auto k_disable  = util::make_byte_data("AT+CIPRECVMODE=0\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( device_type<k_instance>::execute_command(k_expected, std::span{enable ? k_enable : k_disable}) != esp8266::atcode_type::k_ok )
        {
            return tl::make_unexpected(esp8266_error_type::k_multiple_connections);
        }

        return {};
    }

    static auto multiple_connections(bool enable = true) noexcept -> tl::expected<void, esp8266_error_type>
    {
        static constexpr auto k_enable   = util::make_byte_data("AT+CIPMUX=1\r\n");
        static constexpr auto k_disable  = util::make_byte_data("AT+CIPMUX=0\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( device_type<k_instance>::execute_command(k_expected, std::span{enable ? k_enable : k_disable}) != esp8266::atcode_type::k_ok )
        {
            return tl::make_unexpected(esp8266_error_type::k_multiple_connections);
        }

        return {};
    }

    static auto station_mode(esp8266::station_mode mode) noexcept -> tl::expected<void, esp8266_error_type>
    {
        std::array<char, 128> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CWMODE=%ld)" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, static_cast<std::uint32_t>(mode));
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};

        }();

        if ( command.empty() )
        {
            return tl::make_unexpected(esp8266_error_type::k_invalid_argument);
        }

        // To enable/disable Multiple Connections  (AT+CIPMUX=1)
        // Test if the Wi-Fi Mode (Station/SoftAP/Station+SoftAP) supported

        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( device_type<k_instance>::execute_command(k_expected, command) != esp8266::atcode_type::k_ok )
        {
            return tl::make_unexpected(esp8266_error_type::k_echo_off);
        }

        return {};
    }

    static auto echo(bool enable = false) noexcept -> tl::expected<void, esp8266_error_type>
    {
        static constexpr auto k_enable   = util::make_byte_data("ATE1\r\n");
        static constexpr auto k_disable  = util::make_byte_data("ATE0\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( device_type<k_instance>::execute_command(k_expected, std::span{enable ? k_enable : k_disable}) != esp8266::atcode_type::k_ok )
        {
            return tl::make_unexpected(esp8266_error_type::k_echo_off);
        }

        return {};
    }

    static bool software_update()
    {
        static constexpr auto k_command  = util::make_byte_data("AT+CIUPDATE\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        return device_type<k_instance>::execute_command(k_expected, std::span{k_command}) == esp8266::atcode_type::k_ok;
    }

    static auto ipd_info(bool show_remote_info) noexcept -> tl::expected<void, esp8266_error_type>
    {
        static constexpr auto k_command_on  = util::make_byte_data("AT+CIPDINFO=1\r\n");
        static constexpr auto k_command_off = util::make_byte_data("AT+CIPDINFO=0\r\n");
        static constexpr auto k_expected    = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( device_type<k_instance>::execute_command(k_expected, std::span{show_remote_info ? k_command_on : k_command_off}) != esp8266::atcode_type::k_ok )
        {
            return tl::make_unexpected(esp8266_error_type::k_echo_off);
        }

        return {};
    }
};

} // namespace sys::dev::esp8266::cmd
