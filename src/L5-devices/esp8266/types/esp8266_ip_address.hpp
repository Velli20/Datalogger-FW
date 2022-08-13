#pragma once

namespace sys::dev::esp8266 {

struct ip_address_type
{
    std::uint16_t m_address[4];

    [[nodiscard]] bool operator==(const ip_address_type&) const noexcept = default;

    constexpr auto operator<=>(const ip_address_type&) const = default;
};

} // namespace sys::dev::esp8266

namespace sys::log {

template <class T>
constexpr auto operator<<(
    formatting_stream<T>&                os,
    const dev::esp8266::ip_address_type& arg
) noexcept -> formatting_stream<T>&
{
    os << arg.m_address[0] << '.';
    os << arg.m_address[1] << '.';
    os << arg.m_address[2] << '.';
    os << arg.m_address[3];

    return os;
}

} // namespace sys::log
