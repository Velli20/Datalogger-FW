#pragma once

namespace sys::dev::esp8266 {

enum class errc : std::uint32_t
{
    no_error,
    set_station_mode,
    multiple_connections,
    echo_off,
    sntp_time,
    scan_access_points,
    create_access_point,
    deep_sleep_mode,
    disconnect_from_access_point,
    connect_to_access_point,
    enable_sntp_server,
    get_sntp_time,
    socket_open,
    socket_send,
    socket_read,
    at_timeout,
    at_write,
    read_timeout,
    no_ready_response,
    at_command_send,
    invalid_argument,
};

} // namespace sys::dev::esp8266

template <>
struct sys::is_error_code_enum<sys::dev::esp8266::errc> : std::true_type {};

namespace sys::dev::esp8266 {

static constexpr struct esp8266_error_category : public sys::error_category
{
    constexpr esp8266_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"ESP-AT"}; }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<esp8266::errc>(value) )
            {
                case esp8266::errc::no_error:
                    return "No error"sv;

                case esp8266::errc::set_station_mode:
                    return ""sv;

                case esp8266::errc::multiple_connections:
                    return ""sv;

                case esp8266::errc::echo_off:
                    return ""sv;

                case esp8266::errc::sntp_time:
                    return ""sv;

                case esp8266::errc::scan_access_points:
                    return ""sv;

                case esp8266::errc::create_access_point:
                    return ""sv;

                case esp8266::errc::deep_sleep_mode:
                    return ""sv;

                case esp8266::errc::disconnect_from_access_point:
                    return ""sv;

                case esp8266::errc::connect_to_access_point:
                    return ""sv;

                case esp8266::errc::enable_sntp_server:
                    return ""sv;

                case esp8266::errc::get_sntp_time:
                    return ""sv;

                case esp8266::errc::socket_open:
                    return ""sv;

                case esp8266::errc::socket_send:
                    return ""sv;

                case esp8266::errc::socket_read:
                    return ""sv;

                case esp8266::errc::at_timeout:
                    return ""sv;

                case esp8266::errc::at_write:
                    return ""sv;

                case esp8266::errc::no_ready_response:
                    return ""sv;

                case esp8266::errc::read_timeout:
                    return ""sv;

                case esp8266::errc::at_command_send:
                    return ""sv;

                case esp8266::errc::invalid_argument:
                    return ""sv;
            }
        }

        return {};
    }
} k_esp8266_error_category{};

[[nodiscard]] constexpr auto make_error_code(esp8266::errc error)
{
    return sys::error_code{error, k_esp8266_error_category};
}

} // namespace sys::dev::esp8266
