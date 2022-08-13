#pragma once

namespace sys::dev::esp8266 {

enum state_type : std::uint32_t
{
    k_disconnected = 0u,
    k_connecting,
    k_connected,
};

} //namespace sys::dev::esp8266
