#pragma once

namespace sys::dev::esp8266::cmd {

template <device_node k_instance = "/dev/wifi@0">
struct power
{
    static bool set_mode(esp8266::sleep_mode_type mode)
    {
        std::array<char, 128> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+SLEEP=%lu)" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, static_cast<std::uint32_t>(mode));
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
            // m_error = make_error_code(esp8266::errc::deep_sleep_mode);
            return false;
        }

        return true;
    }

    static bool deep_sleep(std::chrono::milliseconds sleep_time)
    {
        std::array<char, 128> buffer;

        auto command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+GSLP=%lld)" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, sleep_time.count());
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
            // m_error = make_error_code(esp8266::errc::deep_sleep_mode);
            return false;
        }

        return true;
    }

};

} // namespace sys::dev::esp8266::cmd
