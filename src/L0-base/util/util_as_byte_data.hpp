#pragma once

namespace sys::util {

template <std::size_t k_length>
constexpr auto as_byte_data(const char (&s)[k_length])
{
    return std::as_bytes(std::span{s, k_length});
}

constexpr auto as_byte_data(const sys::concepts::string_like auto s)
{
    return std::as_bytes(std::span{s.data(), s.size()});
}

template <std::size_t k_length>
consteval auto make_byte_data(const char (&s)[k_length]) noexcept
{
    std::array<std::byte, k_length - 1> a {};
    for ( std::size_t i{}; i < k_length - 1; ++i )
    {
        a[i] = static_cast<std::byte>(s[i]);
    }

    return a;
}

namespace literals {

template <char... k_literals>
consteval inline auto operator"" _bytes() noexcept
{
    return make_byte_data(k_literals...);
}

} // namespace literals

} // namespace sys::util
