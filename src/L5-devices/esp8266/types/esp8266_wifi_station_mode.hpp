#pragma once

namespace sys::dev::esp8266 {

enum class station_mode : std::uint32_t
{
    kNull             = 0u, // -  0: Null mode. Wi-Fi RF will be disabled.
    kStation          = 1u, // -  1: Station mode.
    kSoftAp           = 2u, // -  2: SoftAP mode.
    kSoftApAndStation = 3u, // -  3: SoftAP+Station mode.
};

} // namespace sys::dev::esp8266
