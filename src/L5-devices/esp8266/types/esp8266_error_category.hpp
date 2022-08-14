#pragma once

namespace sys::dev {

enum class esp8266_error_type : std::uint32_t
{
    k_no_error,
    k_set_station_mode,
    k_multiple_connections,
    k_echo_off,
    k_sntp_time,
    k_scan_access_points,
    k_create_access_point,
    k_deep_sleep_mode,
    k_disconnect_from_access_point,
    k_connect_to_access_point,
    k_enable_sntp_server,
    k_get_sntp_time,
    k_socket_open,
    k_socket_send,
    k_socket_read,
    k_at_timeout,
    k_at_write,
    k_read_timeout,
    k_no_ready_response,
    k_at_command_send,
    k_invalid_argument,
};

} // namespace sys::dev

template <>
struct sys::is_error_code_enum<sys::dev::esp8266_error_type> : std::true_type {};

namespace sys::dev {

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

            switch ( static_cast<esp8266_error_type>(value) )
            {
                case esp8266_error_type::k_no_error:
                    return "No error"sv;

                case esp8266_error_type::k_set_station_mode:
                    return ""sv;

                case esp8266_error_type::k_multiple_connections:
                    return ""sv;

                case esp8266_error_type::k_echo_off:
                    return ""sv;

                case esp8266_error_type::k_sntp_time:
                    return ""sv;

                case esp8266_error_type::k_scan_access_points:
                    return ""sv;

                case esp8266_error_type::k_create_access_point:
                    return ""sv;

                case esp8266_error_type::k_deep_sleep_mode:
                    return ""sv;

                case esp8266_error_type::k_disconnect_from_access_point:
                    return ""sv;

                case esp8266_error_type::k_connect_to_access_point:
                    return ""sv;

                case esp8266_error_type::k_enable_sntp_server:
                    return ""sv;

                case esp8266_error_type::k_get_sntp_time:
                    return ""sv;

                case esp8266_error_type::k_socket_open:
                    return ""sv;

                case esp8266_error_type::k_socket_send:
                    return ""sv;

                case esp8266_error_type::k_socket_read:
                    return ""sv;

                case esp8266_error_type::k_at_timeout:
                    return ""sv;

                case esp8266_error_type::k_at_write:
                    return ""sv;

                case esp8266_error_type::k_no_ready_response:
                    return ""sv;

                case esp8266_error_type::k_read_timeout:
                    return ""sv;

                case esp8266_error_type::k_at_command_send:
                    return ""sv;

                case esp8266_error_type::k_invalid_argument:
                    return ""sv;
            }
        }

        return {};
    }
} k_esp8266_error_category{};

[[nodiscard]] constexpr auto make_error_code(esp8266_error_type error)
{
    return sys::error_code{error, k_esp8266_error_category};
}

} // namespace sys::dev
