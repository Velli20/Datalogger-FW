#pragma once

namespace sys::util {

template <std::size_t k_lenght>
constexpr auto hash_fnv1a(const char (&s)[k_lenght])
{
    constexpr std::uint32_t k_fnv_prime  = 0x01000193;
    constexpr std::uint32_t k_fnv_offset = 0x811C9DC5;

    std::uint32_t result = k_fnv_offset;

    for ( char c : s )
    {
        result ^= static_cast<std::uint32_t>(c);
        result *= k_fnv_prime;
    }

    return result;
}

constexpr auto hash_fnv1a(const char* s, std::size_t n)
{
    constexpr std::uint32_t k_fnv_prime  = 0x01000193;
    constexpr std::uint32_t k_fnv_offset = 0x811C9DC5;

    std::uint32_t result = k_fnv_offset;

    for ( std::size_t i{}; i < n; i++ )
    {
        result ^= static_cast<std::uint32_t>(s[i]);
        result *= k_fnv_prime;
    }

    return result;
}

} // namespace sys::util
