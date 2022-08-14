#pragma once

namespace sys::dev::esp8266::cmd {

template <device_node k_instance = "/dev/wifi@0">
struct sntp
{
    static bool now(std::tm& t) noexcept
    {
        using namespace std::chrono_literals;

        static constexpr auto k_command  = util::make_byte_data("AT+CIPSNTPTIME?\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( auto rc = device_type<k_instance>::write(std::span{k_command}); rc != true )
        {
            // m_error = make_error_code(esp8266_error_type::k_at_command_send);
            return false;
        }

        if ( auto response = device_type<k_instance>::wait(esp8266::response_table::make<esp8266::atcode_type::k_sntp_time>::value, 10000ms); response != esp8266::atcode_type::k_sntp_time )
        {
            // m_error = make_error_code(esp8266_error_type::k_get_sntp_time);
            return false;
        }

        if ( auto rc = device_type<k_instance>::read(esp8266::parse::sntp{t}, 10000ms); rc != true )
        {
            // m_error = make_error_code(esp8266_error_type::k_get_sntp_time);
            return false;
        }

        if ( auto response = device_type<k_instance>::wait(k_expected, 10000ms); response != esp8266::atcode_type::k_ok )
        {
            // m_error = make_error_code(esp8266_error_type::k_get_sntp_time);
            return false;
        }

        return true;
    }

    static bool enable(
        std::int32_t           timezone,
        const std::string_view server = std::string_view{""}
    )
    {
        std::array<char, 128> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CIPSNTPCFG=1,%ld,"%s")" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, timezone, server.data());
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
            // m_error = make_error_code(esp8266_error_type::k_enable_sntp_server);
            return false;
        }

        return true;
    }
};

} // namespace sys::dev::esp8266::cmd
