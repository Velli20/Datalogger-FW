#pragma once

namespace sys::algorithm {

namespace detail {

template <typename I> requires std::is_integral_v<I>
constexpr std::size_t k_size_in_values = static_cast<std::size_t>(std::numeric_limits<I>::max()) - std::numeric_limits<I>::lowest() + 1;

template <typename char_type>
constexpr auto k_to_lower_table
{
    [] () constexpr
    {
        std::array<char_type, detail::k_size_in_values<char_type>> ret{};

        for ( std::size_t i = 0; i < detail::k_size_in_values<char_type>; ++i )
        {
            ret[i] = i;
        }

        for ( std::size_t i = char_type{'A'}; i <= char_type{'Z'}; ++i )
        {
            ret[i] = ret[i] - char_type{'A'} + char_type{'a'};
        }

        return ret;
    }()
};

constexpr auto to_lower(sys::concepts::character_like auto c) noexcept
{
    using return_type          = decltype(c);
    using unsigned_return_type = std::make_unsigned_t<return_type>;

    return k_to_lower_table<return_type>[static_cast<unsigned_return_type>(c)];
}

} // namespace detail

[[nodiscard]] constexpr auto iequals(
    sys::concepts::string_like auto& str1,
    sys::concepts::string_like auto& str2
) noexcept -> bool
{
    return std::equal(
        std::cbegin(str1),
        std::cend(str1),
        std::cbegin(str2),
        std::cend(str2),
        [](const auto a, const auto b)
        {
            return detail::to_lower(a) == detail::to_lower(b);
        }
    );
}

[[nodiscard]] constexpr auto iequals(
    sys::concepts::string_like auto&& str1,
    sys::concepts::string_like auto&& str2
) noexcept -> bool
{
    return std::equal(
        std::cbegin(str1),
        std::cend(str1),
        std::cbegin(str2),
        std::cend(str2),
        [](const auto a, const auto b)
        {
            return detail::to_lower(a) == detail::to_lower(b);
        }
    );
}

template <typename T> requires(std::is_integral_v<T>)
[[nodiscard]] auto string_to_integral(
    sys::concepts::string_like auto const& s,
    auto const                             base = 10
) -> std::optional<T>
{
    if ( s.empty() )
        return {};

    T value{};

    if ( auto [ptr, err] = std::from_chars(std::cbegin(s), std::cend(s), value, base); err == std::errc{} )
    {
        return value;
    }

    return {};
}

[[nodiscard]] constexpr auto integral_to_chars(sys::concepts::integral_like auto value) -> std::pair<std::array<char, 21>, std::size_t>
{
    std::array<char, 21> buffer{};

    auto first = buffer.data();
    auto last  = first + buffer.size();

    if ( auto [ptr, err] = std::to_chars(first, last, value); err == std::errc{} )
        return {buffer, ptr - first};

    return {buffer, std::size_t{}};
}

} // namespace sys::algorithm
