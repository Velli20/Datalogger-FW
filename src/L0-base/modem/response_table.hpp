#pragma once

namespace sys::dev {

template <typename T>
struct response
{
    const T                m_code{};
    const std::string_view m_suffix{};

    [[nodiscard]] constexpr auto operator==(const response& other) const noexcept -> bool
    {
        return m_code == other.m_code;
    }

    [[nodiscard]] constexpr auto operator!=(const response& other) const noexcept -> bool
    {
        return not (*this == other);
    }

    [[nodiscard]] constexpr auto text() const noexcept
    {
        return m_suffix;
    }

    [[nodiscard]] constexpr auto code() const noexcept
    {
        return m_code;
    }

    [[nodiscard]] constexpr auto equals(T c) const noexcept
    {
        return m_code == c;
    }

    template <typename Container>
    [[nodiscard]] constexpr auto equals(/* const */ Container& c) const noexcept
    {
        if /* constexpr */ ( m_suffix.empty() == false )
        {
            if ( m_suffix.size() > c.size() )
                return false;

            return std::equal(std::crbegin(m_suffix), std::crend(m_suffix), std::crbegin(c));
        }

        return true;
    }
};

} // namespace sys

