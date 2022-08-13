#pragma once

namespace sys::http {

template <std::size_t k_max_fields = 0>
struct const_request
{
    struct empty_type {};

    using headers_type = std::conditional_t<k_max_fields != 0, std::array<http::field, k_max_fields>, empty_type>;

    explicit consteval const_request(
        std::string_view  url    = std::string_view{"/"},
        http::method_type method = http::method_type::k_get
    )
        : m_url{url}
        , m_method{http::to_string(method)}
        , m_protocol{http::to_string(http::parse::url(m_url).protocol)}
    {
    }

    template <std::size_t k_other_max_fields>
    explicit consteval const_request(
        const const_request<k_other_max_fields>& other,
        std::span<const http::field>             fields
    )
        : m_url{other.m_url}
        , m_method{other.m_method}
        , m_protocol{other.m_protocol}
    {
        if constexpr ( k_max_fields > 0 )
        {
            std::copy(std::begin(fields), std::end(fields), std::begin(m_headers));
        }
    }

    consteval auto add_field(
        std::string_view name,
        std::string_view value
    )
    {
        std::array<http::field, k_max_fields + 1> headers{};

        if constexpr ( k_max_fields > 0 )
        {
            std::copy_n(std::begin(m_headers), k_max_fields, std::begin(headers));
        }

        headers[k_max_fields] = http::field{name, value};

        return const_request<k_max_fields + 1>{*this, std::span{headers}};
    }

    [[nodiscard]] constexpr operator http::request_view() const noexcept
    {
        if constexpr ( k_max_fields == 0 )
        {
            return request_view
            {
                .m_url{http::parse::url(m_url)},
                .m_method{m_method},
                .m_protocol{m_protocol}
            };
        }

        else
        {
            return request_view
            {
                .m_url{http::parse::url(m_url)},
                .m_method{m_method},
                .m_protocol{m_protocol},
                .m_headers{std::span{m_headers}}
            };
        }
    }

    const std::string_view m_url{};
    const std::string_view m_method{};
    const std::string_view m_protocol{};
    headers_type           m_headers{};
};


} // namespace http
