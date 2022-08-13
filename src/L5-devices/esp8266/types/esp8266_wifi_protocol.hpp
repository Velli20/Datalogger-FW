#pragma once

namespace sys::dev::esp8266 {

enum class connection_type : std::uint32_t
{
    k_tcp = 0,
    k_udp,
    k_ssl
};

} // namespace sys::dev::esp8266
