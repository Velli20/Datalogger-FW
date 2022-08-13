
#pragma once

namespace sys::concepts {

template <typename Type>
concept container_like = requires(Type container)
{
    typename std::remove_cvref_t<Type>::value_type;
    container.size();
    container.begin();
    container.end();
};

template <typename T>
concept integral_like = std::is_integral_v<T>;

template <typename T>
concept string_like = std::is_convertible_v<T, std::string_view> ||
                      std::is_convertible_v<T, platform::pmr::string>;

template <typename T>
concept character_like = std::integral<T> &&
                        (std::same_as<T, char> || std::same_as<T, unsigned char> || std::same_as<T, signed char>);


} // namespace sys::concepts
