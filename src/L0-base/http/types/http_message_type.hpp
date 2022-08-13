#pragma once

namespace sys::http {

struct body
{
    using iterator       = io::input_sink_iterator;
    using const_iterator = io::input_sink_iterator;
    using size_type      = std::size_t;

    constexpr body(
        io::input_sink_interface& socket_interface,
        std::size_t               content_length
    )
        : m_socket{socket_interface}
        , m_content_length{content_length}
    {}

    [[nodiscard]] auto begin() -> iterator
    {
        return {m_socket, m_content_length, 0};
    }

    [[nodiscard]] auto end() -> iterator
    {
        return {m_socket, m_content_length, m_content_length};
    }

    [[nodiscard]] auto cbegin() -> iterator
    {
        return {m_socket, m_content_length, 0};
    }

    [[nodiscard]] auto cend() -> iterator
    {
        return {m_socket, m_content_length, m_content_length};
    }

    [[nodiscard]] auto size() const -> size_type
    {
        return m_content_length;
    }

    private:

    io::input_sink_interface& m_socket;
    size_type                 m_content_length{};
};

struct message
{
    using header_type = std::variant<
        http::header,
        http::header_view
    >;

    using body_type = std::optional<http::body>;

    public:

    message() = default;

    explicit message(
        std::string_view uri,
        http::header&&   header
    )
        : m_uri{uri}
        , m_header{std::forward<decltype(header)>(header)}
    {}

    explicit message(
        std::string_view   uri,
        http::start_line&& start_line,
        http::header&&     header
    )
        : m_uri{uri}
        , m_start_line{std::move(start_line)}
        , m_header{std::forward<decltype(header)>(header)}
    {}

    explicit message(
        std::string_view         uri,
        http::method_type        method,
        http::protocol_type      protocol,
        const http::header_view& header
    )
        : m_uri{uri}
        , m_header{header}
    {}

    explicit message(
        std::string_view                uri,
        http::method_type               method,
        http::protocol_type             protocol,
        platform::pmr::memory_resource* memory = platform::memory::default_resource()
    )
        : m_uri{uri}
        , m_header{http::header{memory}}
    {}

    auto set_body(
        io::input_sink_interface& b,
        std::size_t               l
    )
    {
        m_body.emplace(http::body{b, l});
    }

    [[nodiscard]] auto has_body() const
    {
        return m_body.has_value();
    }

    [[nodiscard]] auto body() const
    {
        return m_body.value();
    }

    [[nodiscard]] auto method() const -> http::method_type
    {
        if ( m_start_line.has_method() )
        {
            return m_start_line.method();
        }

        return http::method_type::k_unknown;
    }

    [[nodiscard]] auto append(
        std::string_view key,
        std::string_view value
    ) -> message&
    {
        if ( std::holds_alternative<http::header>(m_header) )
        {
            std::get<http::header>(m_header).fields().reserve(10); /* FIXME */
            std::get<http::header>(m_header).fields().emplace_back(key, value);
        }

        return *this;
    }

    [[nodiscard]] auto url() const
    {
        return m_uri;
    }

    [[nodiscard]] auto fields() const -> std::span<const http::field>
    {
        if ( std::holds_alternative<http::header>(m_header) )
        {
            return std::get<http::header>(m_header).fields();
        }

        return std::get<http::header_view>(m_header).fields();
    }

    [[nodiscard]] auto content_length() const -> std::size_t
    {
        for ( const auto& [name, value] : fields() )
        {
            constexpr auto k_content_length    = std::string_view{"content-length"};
            constexpr auto k_transfer_encoding = std::string_view{"Transfer-Encoding"};
            // constexpr auto k_chunked           = std::string_view{"chunked"};

            if ( sys::algorithm::iequals(name, k_transfer_encoding) /* && sys::algorithm::iequals(value, k_chunked) */ )
            {
                return std::numeric_limits<std::size_t>::max();
            }

            if ( sys::algorithm::iequals(name, k_content_length) == false )
            {
                continue;
            }

            if ( auto length = sys::algorithm::string_to_integral<std::size_t>(value, 10); length.has_value() )
            {
                return length.value();
            }

            return {};
        }

        return {};
    }

    [[nodiscard]] auto content_type() const -> decltype(http::mime_from_string(std::string_view{}))
    {
        for ( const auto& [name, value] : fields() )
        {
            constexpr auto k_content_type = std::string_view{"content-type"};

            if ( sys::algorithm::iequals(name, k_content_type) == false )
            {
                continue;
            }

            return http::mime_from_string(value);
        }

        return {};
    }

    private:

    std::string_view m_uri{};
    http::start_line m_start_line{};
    header_type      m_header{http::header_view{}};
    body_type        m_body{};

};

static inline auto get(
    std::string_view    url,
    http::protocol_type p = http::protocol_type::k_http
) noexcept
{
    return http::message{url, http::method_type::k_get, p};
}

} // namespace sys::http
