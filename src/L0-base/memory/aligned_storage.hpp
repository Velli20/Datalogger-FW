#pragma once

namespace sys {

template<std::size_t k_size, std::size_t k_alignment>
struct alignas(k_alignment) aligned_storage
{
    template<typename T> requires(sizeof(T) <= k_size)
    [[nodiscard]] constexpr auto get() noexcept -> T*
    {
        return std::bit_cast<T*>(data());
    }

    template<typename T> requires(sizeof(T) <= k_size)
    [[nodiscard]] constexpr auto get() const noexcept -> const T*
    {
        return std::bit_cast<const T*>(data());
    }

    [[nodiscard]] constexpr auto data() noexcept -> void*
    {
        return static_cast<void*>(m_bytes);
    }

    [[nodiscard]] constexpr auto data() const noexcept -> const void*
    {
        return static_cast<const void*>(m_bytes);
    }

    private:

    std::byte m_bytes[k_size];
};

template<typename T>
using type_storage = aligned_storage<sizeof(T), alignof(T)>;

} // namespace sys
