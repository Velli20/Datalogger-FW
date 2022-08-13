#pragma once

namespace sys::http {

enum class protocol_type
{
    k_http,
    k_https
};

consteval auto to_string(http::protocol_type code) -> std::string_view
{
    constexpr auto k_http_string  = std::string_view{"HTTP/1.1"};
    constexpr auto k_https_string = std::string_view{"HTTPS/1.1"};

    return code == http::protocol_type::k_http ? k_http_string : k_https_string;
}

} // namespace sys::http
