#pragma once

namespace sys::dev::esp8266::cmd {

template <device_node k_instance = "/dev/wifi@0">
struct dns
{
    static bool set(
        std::string_view primary,
        std::string_view secondary = std::string_view{}
    )
    {
        if ( primary.empty() )
        {
            return false;
        }

        std::array<char, 128> buffer;

        auto command_primary_only = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CIPDNS_DEF=1,"%s")" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, primary.data());
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};
        };

        auto command_w_secondary = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CIPDNS_DEF=1,"%s","%s")" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, primary.data(), secondary.data());
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};
        };

        auto command = secondary.empty() ? command_primary_only() : command_w_secondary();
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

    static bool get(
        platform::pmr::string& primary,
        platform::pmr::string& secondary
    ) noexcept
    {
        using namespace std::chrono_literals;

        static constexpr auto k_command  = util::make_byte_data("AT+CIPDNS_DEF?\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail,
            esp8266::atcode_type::k_query_dns
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( auto rc = device_type<k_instance>::write(std::span{k_command}); rc != true )
        {
            return false;
        }

        bool got_primary   = false;
        bool got_secondary = false;

        const auto parser = [&](auto c) -> bool
        {
            if ( c == '\n' )
            {
                if ( got_primary == false )
                {
                    got_primary = true;
                }

                else if ( got_secondary == false )
                {
                    got_secondary = true;
                }

                return false;
            }

            if ( got_primary == false )
            {
                primary += c;
            }

            else if ( got_secondary == false )
            {
                secondary += c;
            }

            return true;
        };

        while ( true )
        {
            auto response = device_type<k_instance>::wait(k_expected, 2000ms);
            if ( response.has_value() == false )
            {
                return false;
            }

            if ( response.value() == esp8266::atcode_type::k_ok )
            {
                return true;
            }

            if ( response.value() == esp8266::atcode_type::k_error || response.value() == esp8266::atcode_type::k_fail )
            {
                return false;
            }

            if ( response.value() != esp8266::atcode_type::k_query_dns )
            {
                continue;
            }

            if ( device_type<k_instance>::read(parser, 1000ms) != true )
            {
                return false;
            }
        }

        return false;
    }

    static auto resolve(
        std::string_view          host,
        esp8266::ip_address_type& v
    ) noexcept
    {
        using namespace std::chrono_literals;

        std::array<char, 128> buffer;

        auto k_command = [&]() -> std::span<char>
        {
            static constexpr auto k_format = R"(AT+CIPDOMAIN="%s")" "\r\n";

            const auto length = std::snprintf(buffer.data(), buffer.size(), k_format, host.data());
            if ( length > 0 )
            {
                return std::span{std::begin(buffer), static_cast<std::size_t>(length)};
            }

            return {};
        }();

        if ( k_command.empty() )
        {
            return false;
        }

        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail,
            esp8266::atcode_type::k_query_domain
        >::value;

        esp8266::lock<k_instance> lock{};

        if ( auto rc = device_type<k_instance>::write(std::span{k_command}); rc != true )
        {
            return false;
        }

        while ( true )
        {
            auto response = device_type<k_instance>::wait(k_expected, 2000ms);
            if ( response.has_value() == false )
            {
                return false;
            }

            if ( response.value() == esp8266::atcode_type::k_error || response.value() == esp8266::atcode_type::k_fail )
            {
                return false;
            }

            if ( response.value() == esp8266::atcode_type::k_query_domain )
            {
                break;
            }
        }

        if ( auto parser = esp8266::parse::ip{v}; device_type<k_instance>::read(parser, 1000ms) != true )
        {
            return false;
        }

        while ( true )
        {
            auto response = device_type<k_instance>::wait(k_expected, 2000ms);
            if ( response.has_value() == false )
            {
                return false;
            }

            if ( response.value() == esp8266::atcode_type::k_error || response.value() == esp8266::atcode_type::k_fail )
            {
                return false;
            }

            if ( response.value() == esp8266::atcode_type::k_ok )
            {
                return true;
            }
        }
    }
};

} // namespace sys::dev::esp8266::cmd
