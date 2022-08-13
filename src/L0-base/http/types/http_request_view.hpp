#pragma once

namespace sys::http {

struct request_view
{
    auto write(io::output_sink_interface& os) const
    {
        if ( m_method.empty() || m_protocol.empty() )
        {
            return false;
        }

        static constexpr auto k_delimeter = std::string_view{"\r\n"};

        os << util::as_byte_data(m_method)   << std::byte{' '};
        os << util::as_byte_data(m_url.path) << std::byte{' '};
        os << util::as_byte_data(m_protocol) << util::as_byte_data(k_delimeter);

        for ( const auto& [name, value] : m_headers )
        {
            static constexpr auto k_separator = std::string_view{": "};

            os << util::as_byte_data(name)
               << util::as_byte_data(k_separator)
               << util::as_byte_data(value)
               << util::as_byte_data(k_delimeter);
        }

        os << util::as_byte_data(k_delimeter);

        return true;
    }

    [[nodiscard]] constexpr auto host() const
    {
        return m_url.host;
    }

    [[nodiscard]] constexpr auto port() const
    {
        return m_url.port;
    }

    http::url_components         m_url{};
    std::string_view             m_method{};
    std::string_view             m_protocol{};
    std::span<const http::field> m_headers{};
};

} // namespace sys::http
