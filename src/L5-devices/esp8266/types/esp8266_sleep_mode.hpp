#pragma once

namespace sys::dev::esp8266 {

enum class sleep_mode_type : std::uint32_t
{
    k_disable = 0x00u,
    k_light   = 0x01u,
    k_modem   = 0x02u
};

} //namespace sys::dev::esp8266