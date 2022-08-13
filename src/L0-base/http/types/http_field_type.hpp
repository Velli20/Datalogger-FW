#pragma once

namespace sys::http {

struct field
{
    constexpr field() = default;

    explicit constexpr field(
        std::string_view name,
        std::string_view value
    )
        : m_name{name}
        , m_value{value}
        {}

    [[nodiscard]] constexpr auto name() const
    {
        return m_name;
    }

    [[nodiscard]] constexpr auto value() const
    {
        return m_value;
    }

    std::string_view m_name{};
    std::string_view m_value{};
};

} // namespace sys::http
