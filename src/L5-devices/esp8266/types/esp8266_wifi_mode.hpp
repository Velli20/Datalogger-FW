#pragma once

namespace sys::dev::esp8266 {

enum class wifi_mode : std::uint8_t
{
    kModeB = 0b0000'0001,
    kModeG = 0b0000'0010,
    kModeN = 0b0000'0100,

    kModeBg  = kModeB | kModeG,
    kModeBgn = kModeB | kModeG | kModeN,
};

} // namespace sys::dev::esp8266
