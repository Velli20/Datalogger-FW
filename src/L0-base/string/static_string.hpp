#pragma once

namespace sys::string {

template <typename T, std::size_t k_length>
struct static_string
{
    T m_buffer[k_length + 1] = {};

    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::size_t;
    using iterator        = T*;
    using reference       = const T&;
    using pointer         = const T*;
    using const_iterator  = const T*;

    consteval static_string(T ch) noexcept
    {
        m_buffer[0] = ch;
        m_buffer[1] = 0;
    }

    consteval static_string(const T (&str)[k_length + 1]) noexcept
    {
        std::copy_n(str, k_length, m_buffer);
    }

    [[nodiscard]] constexpr auto empty() const noexcept -> bool
    {
        return k_length == 0;
    }

    [[nodiscard]] constexpr auto size() const noexcept -> size_type
    {
        return k_length;
    }

    [[nodiscard]] constexpr auto data() const noexcept -> pointer
    {
        return m_buffer;
    }

    [[nodiscard]] constexpr auto c_str() const noexcept -> pointer
    {
        return data();
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
    {
        return data();
    }

    [[nodiscard]] constexpr auto end() const noexcept -> const_iterator
    {
        return data() + size();
    }

    [[nodiscard]] constexpr operator std::string_view() const
    {
        return std::string_view{c_str(), k_length};
    }

    [[nodiscard]] constexpr auto operator[](difference_type index) const noexcept -> reference
    {
        return data()[index];
    }

    [[nodiscard]] constexpr auto operator[](difference_type index) noexcept -> value_type
    {
        return data()[index];
    }

    [[nodiscard]] constexpr auto operator<=>(const static_string& other) const = default;

    template <typename U, std::size_t k_other_length> requires(std::is_same<U, T>::value)
    [[nodiscard]] constexpr auto operator==(const static_string<U, k_other_length>& other) const -> bool
    {
        if constexpr ( k_length != k_other_length )
        {
            return false;
        }

        else
        {
            return std::equal(begin(), end(), other.begin());
        }

    }
};

template <typename T, std::size_t k_length>
static_string(const T (&str)[k_length]) -> static_string<T, k_length - 1>;

template <typename T>
static_string(T) -> static_string<T, 1>;

} // namespace sys

