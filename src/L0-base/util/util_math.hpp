#pragma once

namespace mathutils::builtins {

#if __has_builtin(__builtin_clzll)

[[nodiscard]]
constexpr std::uint64_t leading_zeros(std::uint64_t v) noexcept
{
    return v == 0ull ? 64ull : static_cast<std::uint64_t>(__builtin_clzll(v));
}

[[nodiscard]]
constexpr std::uint32_t leading_zeros(std::uint32_t v) noexcept
{
    return v == 0ul ? 32ul : static_cast<std::uint32_t>(__builtin_clz(v));
}

#else

// Based on code from
// https://graphics.stanford.edu/~seander/bithacks.html
[[nodiscard]]
constexpr std::uint32_t leading_zeroes(std::uint64_t v) noexcept
{
    static constexpr char k_bit_positions[64] =
    {
        0,  1,  2,  7,  3,  13, 8,  19, 4,  25, 14, 28, 9,  34, 20, 40,
        5,  17, 26, 38, 15, 46, 29, 48, 10, 31, 35, 54, 21, 50, 41, 57,
        63, 6,  12, 18, 24, 27, 33, 39, 16, 37, 45, 47, 30, 53, 49, 56,
        62, 11, 23, 32, 36, 44, 52, 55, 61, 22, 43, 51, 60, 42, 59, 58
    };

    v |= v >> 1U; // first round down to one less than a power of 2
    v |= v >> 2U;
    v |= v >> 4U;
    v |= v >> 8U;
    v |= v >> 16U;
    v |= v >> 32U;
    v  = ( v >> 1U ) + 1U;

    return 63U - static_cast<std::uint32_t>(k_bit_positions[( v * 0x021'8a39'2cd3'd5dbf ) >> 58U] ); // [3]
}

#endif

} // namespace builtins

namespace mathutils {

template <typename T>
struct log10
{
    constexpr explicit log10(std::uint64_t v)
    {
        value = (64 - builtins::leading_zeros(static_cast<std::uint64_t>(v | 1))) * 1233 >> 12;
    }

    constexpr explicit log10(std::uint32_t v)
    {
        value = (32 - builtins::leading_zeros(static_cast<std::uint32_t>(v | 1))) * 1233 >> 12;
    }

    constexpr ~log10()                        noexcept = default;
    constexpr log10(log10 const&)             noexcept = default;
    constexpr log10(log10&&)                  noexcept = default;

    constexpr log10& operator=(log10 const&)  noexcept = default;
    constexpr log10& operator=(log10&&)       noexcept = default;

    constexpr T get() const { return value; }

    T value;
};

template <typename T> requires(std::is_same_v<T, std::uint64_t> || std::is_same_v<T, std::uint32_t>)
constexpr T pow10(log10<T> n) noexcept
{
    constexpr auto k_range = std::is_same_v<T, std::uint64_t> ? 20 : 10;

    // 10^1 - 10^N

    constexpr auto k_powers_of_tens =
    [] <T... Is> (std::integer_sequence<T, Is...>)
    {
        constexpr auto pow10impl = [](T exp)
        {
            std::size_t result = 1ull;
            for ( ; exp > 0; --exp )
            {
                result *= 10ull;
            }

            return result;
        };

        return std::array<T, sizeof...(Is)> { { pow10impl(Is)...} };

    } (std::make_integer_sequence<T, k_range>{});

    static_assert(k_powers_of_tens[9] == 1000000000u);

    return k_powers_of_tens[n.get()];
}

} // namespace mathutils

