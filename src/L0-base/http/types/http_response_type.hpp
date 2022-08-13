#pragma once

namespace sys::http {

struct file_resource
{
    constexpr file_resource(std::string_view path)
        : m_path{path}
        {}

    std::string_view m_path{};
};

struct response_view
{
    using body_type = std::string_view;

    consteval response_view(
        http::status_code_type code = http::status_code_type::k_ok,
        std::string_view       body = std::string_view{}
    )
        : m_code{code}
        , m_body{body}
        , m_status{http::to_string(code)}
    {}

    [[nodiscard]] auto body() const -> std::string_view
    {
        return m_body;
    }

    auto write(io::output_sink_interface& os) const
    {
        constexpr auto k_http      = http::to_string(http::protocol_type::k_http);
        constexpr auto k_delimeter = std::string_view{"\r\n"};

        os << util::as_byte_data(k_http) << std::byte{' '};
        os << util::as_byte_data(m_status);
        os << util::as_byte_data(k_delimeter);

        if ( m_body.empty() == false )
        {
            constexpr auto k_content = std::string_view{"Content-Length: "};

            auto [chars, len] = sys::algorithm::integral_to_chars(m_body.size());

            os << util::as_byte_data(k_content);
            os << util::as_byte_data(std::string_view{chars.data(), len});
            os << util::as_byte_data(k_delimeter);
        }

        os << util::as_byte_data(k_delimeter);

        if ( m_body.empty() == false )
        {
            os << util::as_byte_data(m_body);
        }

        return true;
    }

    private:

    const http::status_code_type m_code{};
    const body_type         m_body;
    const std::string_view  m_status{};
};

struct response
{
    explicit response(
        http::status_code_type code,
        http::response_body&&  body,
        bool                   chunked = false
    )
        : m_code{code}
        , m_body{std::forward<decltype(body)>(body)}
        , m_chunked{chunked}
    {}

    [[nodiscard]] auto body() const -> const http::response_body&
    {
        return m_body;
    }

    [[nodiscard]] auto code() const
    {
        return m_code;
    }

    auto write(io::output_sink_interface& os) const
    {}

    private:

    http::status_code_type m_code{};
    http::response_body    m_body{};
    bool                   m_chunked{};
};

using router_response_type = std::variant<
    http::response,
    http::response_view
>;

} // namespace sys::http
