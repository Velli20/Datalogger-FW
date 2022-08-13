#pragma once

namespace sys::dev::concepts {

template <typename Func>
concept socket_callback = requires(Func f)
{
    { std::invoke(f, char{}) } -> std::same_as<bool>;
};

template <typename Func>
concept access_point_found_callback = requires(Func f, const esp8266::ap_info_type& ap)
{
    { std::invoke(f, ap) } -> std::same_as<void>;
};

template <typename Func>
concept access_point_client_info_callback = requires(Func f, esp8266::ip_address_type ip, esp8266::mac_address mac)
{
    { std::invoke(f, ip, mac) } -> std::same_as<void>;
};

template <typename Func>
concept http_response_callback = requires(Func f)
{
    { std::invoke(f, std::span<const std::byte>{}) } -> std::same_as<void>;
};

} // namespace sys::dev::concepts
