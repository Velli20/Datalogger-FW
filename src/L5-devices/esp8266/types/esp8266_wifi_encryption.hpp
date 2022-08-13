#pragma once

namespace sys::dev::esp8266 {

enum class encryption_type : std::uint32_t
{
    k_open = 0,
    k_wep,
    k_wpa_psk,
    k_wpa2_psk,
    k_wpa_wap2_psk,
    k_wpa_enterprise
};

} // namespace sys::dev::esp8266
