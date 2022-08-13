#pragma once

namespace sys::io::concepts {

template <typename T>
concept writable = requires(T type)
{
    { type << std::span<std::byte>{} } -> std::same_as<T&>;
    { type << std::byte{}            } -> std::same_as<T&>;
};

template <typename T>
concept readable = requires(T type)
{
    { type >> std::span<std::byte>{} } -> std::same_as<T&>;
    { type >> std::byte{}            } -> std::same_as<T&>;
};

template <typename T>
concept closable = requires(T& type)
{
    type.close();
};

} // namespace sys::concepts
