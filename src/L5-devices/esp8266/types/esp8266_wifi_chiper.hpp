#pragma once

namespace sys::dev::esp8266 {

enum class chiper_type : std::uint32_t
{
    k_none = 0,
    k_wep40,
    k_wep104,
    k_tkip,
    k_ccmp,
    k_tkip_ccmp,
    k_unkown,
};

} // namespace sys::dev::esp8266
