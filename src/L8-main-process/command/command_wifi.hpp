#pragma once

namespace sys {

template <>
struct shell::command_type<"wifi-scan"> final
{
    /** Called with nothing ¯\_(ツ)_/¯ */
    static auto on_command_match(const shell::arguments& argv, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        os << "Scanning for WiFi access points..."sv << log::endl;

        std::size_t count{};

        auto callback = [&](const dev::esp8266::ap_info_type& ap)
        {
            os << "Accees point["sv << count << ']' << log::endl << ap << log::endl << log::endl;
        };

        if ( device_type<"/dev/wifi@0">::list_access_points(callback) != true)
        {
            os << "Failed"sv << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Scan WiFi access points"sv;
    }
};

template <>
struct shell::command_type<"wifi-ap-clients"> final
{
    static auto on_command_match(const shell::arguments& argv, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        os << "Connected clients:"sv << log::endl;

        std::size_t count{};

        auto callback = [&](const dev::esp8266::ip_address_type& ip, const dev::esp8266::mac_address& mac)
        {
            os << "Client["sv << count << ']' << log::endl << ip << log::endl << mac << log::endl;
        };

        if ( device_type<"/dev/wifi@0">::list_clients(callback) != true )
        {
            os << "Failed" << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "List WiFi access point clients"sv;
    }
};

template <>
struct shell::command_type<"wifi-time"> final
{
    static auto on_command_match(const shell::arguments& argv, log::formatting_stream<shell::input_line>& os)
    {
        using namespace std::string_view_literals;

        os << "Requesting SNTP time..."sv << log::endl;

        std::tm t{};
        if ( sys::dev::device_type<"/dev/wifi@0">::get_sntp_time(t) != true )
        {
            os << "Failed to get SNTP time."sv << log::endl;
        }

        else
        {
            os << "SNTP time: "sv << t << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Get SNTP time"sv;
    }

    static consteval auto name() { return std::string_view{"wifi-time"}; }
};

template <>
struct shell::command_type<"wifi-http-get"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;
        using namespace std::string_view_literals;

        if ( args.empty() )
        {
            os << "Expected the URL"sv << log::endl;
            return;
        }

        os << "Requesting URL:"sv << args[0] << log::endl;

        // TODO: http::client tt{google.com};
        // tt.connect("/");

        static constexpr auto k_req = http::const_request
        {
            "http://httpbin.org/get"sv,
            http::method_type::k_get
        }.operator http::request_view();

        auto response = device_type<"/dev/wifi@0">::write(k_req);
        for ( const auto& [name, value] : response.fields() )
        {
            os << name << ": " << value << log::endl;
        }

        if ( response.has_body() )
        {
            for ( auto c : response.body() )
            {
                os << std::to_integer<char>(c);
            }
        }

        else
        {
            os << "No response body"sv << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Make HTTP GET Request. <url>"sv;
    }
};

template <>
struct shell::command_type<"wifi-ap-connect"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        if ( args.empty() )
        {
            os << "Expected the SSID"sv << log::endl;
            return;
        }

        os << "Connecting to " << args[0];

        if ( args.size() > 1 )
        {
            os << " with password "sv << args[1];
        }

        os << log::endl;

        if ( device_type<"/dev/wifi@0">::connect_to_access_point(/* "a-ww3-14hs-2"sv */ args[0], args[1]) )
        {
            os << "WiFi connected"sv << log::endl;
            return;
        }

        os << "Failed to connect to an AP."sv << log::endl;
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Connect to an access point. <SSID>"sv;
    }
};

template <>
struct shell::command_type<"wifi-ap-start"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        if ( args.empty() )
        {
            os << "Expected the SSID"sv << log::endl;
            return;
        }

        auto enc = args.size() > 1 ? dev::esp8266::encryption_type::k_wpa2_psk : dev::esp8266::encryption_type::k_open;

        if ( device_type<"/dev/wifi@0">::create_access_point(args[0], enc, args[1]) == false )
        {
            os << "Failed to create an AP."sv << log::endl;
            return;
        }

        os << "WiFi AP created"sv << log::endl;

        device_type<"/dev/wifi@0">::start(80, dev::esp8266::connection_type::k_tcp);
        // log << open http://http://192.168.4.1
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Create an access point. <SSID> [<PASS>]"sv;
    }
};

template <>
struct shell::command_type<"wifi-ap-disconnect"> final
{
    static auto on_command_match(const shell::arguments&, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        if ( device_type<"/dev/wifi@0">::disconnect_from_access_point() )
        {
            os << "WiFi AP disconnected"sv << log::endl;
            return;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Disconnect from an access point."sv;
    }
};

template <>
struct shell::command_type<"wifi-deep-sleep"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        if ( args.empty() )
        {
            os << "Expected the sleep time"sv << log::endl;
            return;
        }

        auto sleep_time = std::atoi(args[0].data());
        if ( device_type<"/dev/wifi@0">::enter_deep_sleep(std::chrono::milliseconds(sleep_time)) )
        {
            return;
        }

        os << "Failed to enter the deep sleep."sv << log::endl;
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Enter deep sleep for <millis>"sv;
    }
};

template <>
struct shell::command_type<"wifi-ip"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        if ( auto ip = device_type<"/dev/wifi@0">::get_ip_address(); ip.has_value() )
        {
            os << ip.value() << log::endl;
        }

        else
        {
            os << "Failed to get IP address."sv << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Get station ip address"sv;
    }
};

} // namespace sys
