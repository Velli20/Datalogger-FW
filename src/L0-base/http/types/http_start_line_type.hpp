#pragma once

namespace sys::http {

struct start_line
{
    constexpr start_line() = default;

    explicit constexpr start_line(
        std::optional<http::method_type>&& method,
        std::uint32_t                      version
    )
        : m_method{std::forward<decltype(method)>(method)}
        , m_code{std::nullopt}
        , m_version{version}
    {}

    explicit constexpr start_line(
        std::optional<http::method_type>&& method,
        std::uint32_t                      version,
        http::status_code_type             code
    )
        : m_method{std::forward<decltype(method)>(method)}
        , m_code{code}
        , m_version{version}
    {}

    [[nodiscard]] constexpr auto version() const
    {
        return m_version;
    }

    [[nodiscard]] constexpr auto code() const
    {
        return m_code.value();
    }

    [[nodiscard]] constexpr auto has_code() const
    {
        return m_code.has_value();
    }

    [[nodiscard]] constexpr auto has_method() const
    {
        return m_method.has_value();
    }

    [[nodiscard]] constexpr auto method() const
    {
        return m_method.value();
    }

    private:

    std::optional<http::method_type>      m_method{};
    std::optional<http::status_code_type> m_code{};
    std::uint32_t                         m_version{};
};

} // namespace sys::http
