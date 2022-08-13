#pragma once

namespace sys::log::detail {

template <typename T, std::size_t N, typename U> requires (std::is_integral<U>::value)
constexpr auto to_hex_string(
    std::array<T, N>& buffer,
    U                 value,
    bool              upper_case,
    bool show_base
) noexcept
{
    constexpr char k_base_address[] = {'0', 'x'};
    constexpr auto k_max    = (std::numeric_limits<U>::digits + 1) / 4u;

    static_assert(N >= k_max + sizeof(k_base_address), "Invalid output buffer size");

    const std::size_t offset = show_base ? sizeof(k_base_address) : 0u;
    if ( show_base )
    {
        std::copy(std::begin(k_base_address), std::end(k_base_address), std::begin(buffer));
    }

    [&] <std::size_t... Is> (std::integer_sequence<std::size_t, Is...>)
    {
        ([&]
        {
            buffer[offset + k_max - 1 - Is] = upper_case ? "0123456789ABCDEF"[value & 0xF] :
                                                           "0123456789abcdef"[value & 0xF];
            value >>= 4;
        }(), ...);
    }(std::make_integer_sequence<std::size_t, k_max>{});

    buffer[k_max + offset] = '\0';

    return k_max + offset;
}

template <typename T, std::size_t N, typename U> requires (std::is_integral<U>::value)
constexpr auto to_binary_string(
    std::array<T, N>& buffer,
    U                 value,
    const bool        show_base
) noexcept -> std::size_t
{
    constexpr char k_base_address[]    = {'0', 'b'};
    constexpr char k_decimal_separator = '\'';
    constexpr auto k_max               = (std::numeric_limits<U>::digits);
    constexpr auto kDecimalSeparators  = ((k_max + 1) / 4) - 1;

    static_assert(N >= k_max + sizeof(k_base_address) + kDecimalSeparators, "Invalid output buffer size");

    auto  base_offset = show_base ? sizeof(k_base_address) : 0u;
    auto* ptr         = std::addressof(buffer[k_max - 1 + kDecimalSeparators + base_offset]);

    for ( std::size_t i{} ; i < k_max; ++i )
    {
        if ( i != 0 && i != k_max - 1 && (i & 3) == 0 )
        {
            *ptr-- = k_decimal_separator;
        }

        *ptr-- = "01"[value & 0x1];
        value >>= 1;
    }

    if ( show_base )
    {
        std::copy(std::begin(k_base_address), std::end(k_base_address), std::begin(buffer));
    }

    buffer[k_max + kDecimalSeparators + base_offset] = '\0';

    return k_max + kDecimalSeparators + base_offset;
}

template <typename T, std::size_t N, typename U> requires (std::is_integral<U>::value)
constexpr auto to_integer_string(
    std::array<T, N>& buffer,
    U                 value
) noexcept -> std::size_t
{
    constexpr auto k_max          = std::numeric_limits<U>::digits;
    constexpr auto k_base_address = U{10};

    static_assert(N >= k_max, "Invalid output buffer size");

    if ( value == U{0} )
    {
        buffer[0] =  '0';
        buffer[1] = '\0';

        return 2;
    }

    std::size_t offset{};

    if constexpr ( std::is_signed<U>::value )
    {
        if ( value < 0 )
        {
            offset    = 1;
            value     = -value;
            buffer[0] = '-';
        }
    }

    const auto log_10 = mathutils::log10<std::uint32_t>(static_cast<std::uint32_t>(value));
    const auto end    = offset + log_10.get() + (static_cast<std::uint32_t>(value) < mathutils::pow10(log_10) ? 0u : 1u);

    for ( auto i = end; i != offset; --i )
    {
        buffer[i - 1] = value % k_base_address + '0';
        value /= k_base_address;
    }

    buffer[end] = '\0';

    return static_cast<std::size_t>(end);
}

} // namespace sys::log::detail

namespace sys::log {

template <class T, typename U> requires (std::is_integral<U>::value && std::negation<std::is_same<U, bool>>::value)
constexpr auto operator<<(
    log::formatting_stream<T>& os,
    U                          arg
) noexcept -> log::formatting_stream<T>&
{
    auto buffer = std::array<char, std::numeric_limits<U>::digits + 3 + 8>{};

    if ( os.GetIntegralFormat() == format_flag::k_hex_format )
    {
        auto n = detail::to_hex_string(buffer, arg, os.IsFormatUpperCase(), os.IsFormatShowBase());
        os.out(std::string_view{buffer.data(), n});
    }

    else if ( os.GetIntegralFormat() == format_flag::k_bin_format )
    {
        auto n = detail::to_binary_string(buffer, arg, os.IsFormatShowBase());
        os.out(std::string_view{buffer.data(), n});
    }

    else
    {
        auto n = detail::to_integer_string(buffer, arg);
        os.out(std::string_view{buffer.data(), n});
    }

    return os;
}

} // namespace sys::log
