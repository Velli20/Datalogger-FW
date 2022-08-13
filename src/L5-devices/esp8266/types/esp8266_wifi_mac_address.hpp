#pragma once

namespace sys::dev::esp8266 {

struct mac_address
{
    std::uint8_t m_address[6];
};

} // namespace sys::dev::esp8266

namespace sys::log {

template <class T>
[[maybe_unused]] constexpr auto operator<<(formatting_stream<T>& os, const dev::esp8266::mac_address& arg) noexcept -> formatting_stream<T>&
{
    os << log::format::hex << arg.m_address[0] << log::format::dec << ':';
    os << log::format::hex << arg.m_address[1] << log::format::dec << ':';
    os << log::format::hex << arg.m_address[2] << log::format::dec << ':';
    os << log::format::hex << arg.m_address[3] << log::format::dec << ':';
    os << log::format::hex << arg.m_address[4] << log::format::dec << ':';
    os << log::format::hex << arg.m_address[5] << log::format::dec;

    return os;
}

} // namespace sys::log
