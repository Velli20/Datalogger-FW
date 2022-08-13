#pragma once

namespace sys::http::parse {

namespace detail {

struct HostAndPort
{
    std::string_view             host{};
    std::optional<std::uint16_t> port{};
};

[[nodiscard]] constexpr auto split_domain_name(std::string_view domain_name)
{
    if ( auto const colon_position = domain_name.rfind(':'); colon_position != std::string_view::npos )
    {
        if ( auto port = sys::algorithm::string_to_integral<std::uint16_t>(domain_name.substr(colon_position + 1), 10) )
        {
            return HostAndPort
            {
                .host{domain_name.substr(0, colon_position)},
                .port{port}
            };
        }

        return HostAndPort
        {
            .host{domain_name.substr(0, colon_position)}
        };
    }

    return HostAndPort
    {
        .host{domain_name}
    };
}

} // namespace detail

[[nodiscard]] constexpr auto url(std::string_view url) -> http::url_components
{
    using namespace std::string_view_literals;

    auto protocol_to_port = [](http::protocol_type p) -> std::uint16_t
    {
        if ( p == http::protocol_type::k_http  ) { return 80u;  }
        if ( p == http::protocol_type::k_https ) { return 443u; }

        return {};
    };

    auto protocol_from_string = [](std::string_view s) -> http::protocol_type
    {
        if ( sys::algorithm::iequals(s, "http"sv) )  { return http::protocol_type::k_http;  }
        if ( sys::algorithm::iequals(s, "https"sv) ) { return http::protocol_type::k_https; }

        return {};
    };

    if ( url.empty())
    {
        return {};
    }

    auto result = http::url_components{};

    constexpr auto k_whitespace_characters = " \t\r\n"sv;

    // Find the start position of the protocol.

    auto start_position = url.find_first_not_of(k_whitespace_characters);
    if ( start_position == std::string_view::npos )
    {
        return {};
    }

    constexpr auto k_protocol_suffix = "://"sv;

    // Find the end position of the protocol.

    if ( auto position = url.find(k_protocol_suffix, start_position); position != std::string_view::npos )
    {
        result.protocol = protocol_from_string(url.substr(start_position, position - start_position));
        result.port     = protocol_to_port(result.protocol);

        // The start position of the domain name.

        start_position = position + k_protocol_suffix.length();
    }

    // Find the end position of the domain name and start of the path.

    if ( auto position = url.find('/', start_position); position != std::string_view::npos )
    {
        auto [host, port] = detail::split_domain_name(url.substr(start_position, position - start_position));
        if ( port.has_value() )
        {
            result.port = port.value();
        }

        result.host = host;

        start_position = position;
    }

    // There was nothing after the domain name.

    else
    {
        auto [host, port] = detail::split_domain_name(url.substr(start_position));
        if ( port.has_value() )
        {
            result.port = port.value();
        }

        result.host = host;
        result.path = "/"sv;

        return result;
    }

    // Find the end position of the path.

    auto const end_position = url.find_last_not_of(k_whitespace_characters) + 1;
    result.path = url.substr(start_position, end_position - start_position);
    return result;
}

} // namespace sys::http::parse
