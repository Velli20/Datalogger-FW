#pragma once

namespace sys {

template <typename T>
struct is_bitmask_enum : std::false_type {};

template <typename T> requires(is_bitmask_enum<T>::value)
constexpr auto operator^=(T& lhs, T rhs) noexcept
{
    using type = typename std::underlying_type<T>::type;

    lhs = static_cast<T>(static_cast<type>(lhs) ^ static_cast<type>(rhs));
    return lhs;
}

template <typename T> requires(is_bitmask_enum<T>::value)
constexpr auto operator|(T lhs, T rhs) noexcept
{
    using type = typename std::underlying_type<T>::type;

    return static_cast<T>(static_cast<type>(lhs) | static_cast<type>(rhs));
}

template <typename T> requires(is_bitmask_enum<T>::value)
constexpr auto operator|=(T& lhs, T rhs) noexcept
{
    using type = typename std::underlying_type<T>::type;

    lhs = static_cast<T>(static_cast<type>(lhs) | static_cast<type>(rhs));
    return lhs;
}

template <typename T> requires(is_bitmask_enum<T>::value)
constexpr auto operator&=(T& lhs, T rhs) noexcept
{
    using type = typename std::underlying_type<T>::type;

    lhs = static_cast<T>(static_cast<type>(lhs) & static_cast<type>(rhs));
    return lhs;
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator&(T lhs, T rhs) noexcept
{
    using type = typename std::underlying_type<T>::type;

    return static_cast<T>(static_cast<type>(lhs) & static_cast<type>(rhs));
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator^(T lhs, T rhs) noexcept
{
    using type = typename std::underlying_type<T>::type;

    return static_cast<T>(static_cast<type>(lhs) ^ static_cast<type>(rhs));
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator~(T rhs) noexcept
{
    using type = typename std::underlying_type<T>::type;

    return static_cast<T>(~static_cast<type>(rhs));
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto has_flag(T value, T mask) noexcept
{
    using type = typename std::underlying_type<T>::type;

    return static_cast<type>(value & mask) == static_cast<type>(mask) ? true : false;
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto has_any_flag(T value, T mask) noexcept
{
    using type = typename std::underlying_type<T>::type;

    return (static_cast<type>(value & mask) != 0 ? true : false);
}

} // namespace sys
