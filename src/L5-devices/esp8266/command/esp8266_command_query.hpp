#pragma once

namespace sys::dev::esp8266::cmd {

template <device_node k_instance = "/dev/wifi@0">
struct query
{
    static bool version()
    {
        using namespace std::chrono_literals;

        static constexpr auto k_command  = util::make_byte_data("AT+GMR\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        return device_type<k_instance>::execute_command(k_expected, std::span{k_command}) == esp8266::atcode_type::k_ok;
    }

    static bool command_list(bool enable = true)
    {
        static constexpr auto k_command  = util::make_byte_data("+CMD?\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail
        >::value;

        esp8266::lock<k_instance> lock{};

        return device_type<k_instance>::execute_command(k_expected, std::span{k_command}) == esp8266::atcode_type::k_ok;
    }

    static bool access_point_ip(esp8266::ip_address_type& v)
    {
        using namespace std::chrono_literals;

        static constexpr auto k_command  = util::make_byte_data("AT+CIFSR\r\n");
        static constexpr auto k_expected = esp8266::response_table::make<
            esp8266::atcode_type::k_ok,
            esp8266::atcode_type::k_error,
            esp8266::atcode_type::k_fail,
            esp8266::atcode_type::k_query_station_mac,
            esp8266::atcode_type::k_query_accesspoint_mac,
            esp8266::atcode_type::k_query_accesspoint_ip,
            esp8266::atcode_type::k_query_station_ip
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

            if ( response.value() == esp8266::atcode_type::k_query_accesspoint_ip )
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
