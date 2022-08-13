#pragma once

namespace sys::http::parse {

struct header
{
    static constexpr auto k_crlf = std::string_view{"\r\n"};

    explicit header(platform::pmr::memory_resource* memory)
        : m_buffer{memory}
        , m_memory{memory}
    {
        m_buffer.reserve(512);
    }

    [[maybe_unused]] auto operator()(char c) -> bool
    {
        constexpr auto k_end_delimeter = std::array<char, 4>{ '\r', '\n', '\r', '\n' };

        m_buffer.emplace_back(c);

        // Check if we have reached end of the http header.

        if ( c == '\n' )
        {
            auto r = std::find_end(std::begin(m_buffer), std::end(m_buffer), std::cbegin(k_end_delimeter), std::cend(k_end_delimeter));
            if ( r != std::cend(m_buffer) )
            {
                // replace final end delimeter with a null bytes.

                std::replace_if(r, m_buffer.end(), [](auto v) { return v == '\r' or v == '\n'; }, '\0');
                return false;
            }
        }

        return m_buffer.size() < m_buffer.max_size();
    }

    [[nodiscard]] auto field_count() const -> std::size_t
    {
        if ( auto line = fields_string(); line.empty() == false )
        {
            return static_cast<std::size_t>(std::count(line.cbegin(), line.cend(), ':'));
        }

        return {};
    }

    [[nodiscard]] auto finalize() -> std::optional<http::message>
    {
        auto count = field_count();
        if ( count == 0 )
        {
            return {};
        }

        auto line = fields_string();
        if ( line.empty() )
        {
            return {};
        }

        auto res = http::header{m_memory};
        if ( res.fields().reserve(count); res.fields().capacity() != count )
        {
            return {};
        }

        /* https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers

            An HTTP header consists of its case-insensitive name followed by a colon (:),
            then by its value. Whitespace before the value is ignored.
        */

        const auto do_parse = [&]() -> bool
        {
            constexpr auto k_whitespace = std::string_view{" \t\r"};

            auto colon_pos = line.find(':');
            if ( colon_pos == std::string_view::npos )
            {
                return false;
            }

            auto value_start = line.find_first_not_of(k_whitespace, colon_pos + 1);
            auto value_end   = line.find(k_crlf);

            if ( value_start == std::string_view::npos )
                return false;

            res.fields().emplace_back(line.substr(0, colon_pos), value_end == std::string_view::npos ?
                line.substr(value_start) :
                line.substr(value_start, value_end - value_start));

            line = line.substr(value_end + 2);

            return true;
        };

        for ( std::size_t i{}; i != count; ++i )
        {
            if ( do_parse() == false )
            {
                break;
            }
        }

        if ( auto s = start_line(); s.has_value() )
        {
            return http::message{uri(), std::move(s.value()), std::move(res)};
        }

        return http::message{uri(), std::move(res)};
    }

    [[nodiscard]] auto uri() const -> std::string_view
    {
        constexpr auto k_whitespace = std::string_view{" \t\r"};

        auto line = raw();
        if ( line.empty() )
        {
            return {};
        }

        auto redirection_start = line.find('/', 0);
        if ( redirection_start == std::string_view::npos )
        {
            return {};
        }

        auto redirection_end = line.find_first_of(k_whitespace, redirection_start);
        if ( redirection_end == std::string_view::npos )
        {
            return {};
        }

        return line.substr(redirection_start, redirection_end - redirection_start);
    }

    [[nodiscard]] auto method() const -> std::optional<http::method_type>
    {
        constexpr auto k_whitespace = std::string_view{" \t\r"};

        auto line = raw();
        if ( line.empty() )
        {
            return {};
        }

        auto method_end = line.find_first_of(k_whitespace, 0);
        if ( method_end == std::string_view::npos )
        {
            return {};
        }

        return http::method_from_string(line.substr(0, method_end));
    }

    [[nodiscard]] auto start_line() const -> std::optional<http::start_line>
    {
        auto line = status_line_string();
        if ( line.empty() )
        {
            return {};
        }

        static constexpr auto k_prefix     = std::string_view{"HTTP/1."};
        static constexpr auto k_whitespace = std::string_view{" \t\r"};

        // Find HTTP-version start by looking for "HTTP/1." prefix.

        auto version_start = line.find(k_prefix);
        if ( version_start == std::string_view::npos )
        {
            return {};
        }

        version_start += k_prefix.size();

        // Find HTTP-version end by looking for first occurance of whitespace charachter.

        auto version_end = line.find_first_of(k_whitespace, version_start);
        if ( version_end == std::string_view::npos )
        {
            version_end = line.size();
        }

        // Parse version integer value.

        auto version = [&]()
        {
            auto version_string = line.substr(version_start, version_end - version_start);

            return sys::algorithm::string_to_integral<std::uint32_t>(version_string, 10);
        }();

        if ( version.has_value() == false )
        {
            return {};
        }

        if ( version_end == line.size() )
        {
            return http::start_line{ method(), version.value() };
        }

        // Parse status code value.

        const auto status_code_type = [](auto l) -> std::optional<std::uint32_t>
        {
            // Find status code start...

            auto status_code_start = l.find_first_not_of(k_whitespace);
            if ( status_code_start == std::string_view::npos )
            {
                return {};
            }

            // ... and end.

            auto status_code_end = l.find_first_of(k_whitespace, status_code_start);
            if ( status_code_end == std::string_view::npos )
            {
                return {};
            }

            auto status_code_string = l.substr(status_code_start, status_code_end);

            return sys::algorithm::string_to_integral<std::uint32_t>(status_code_string, 10);
        }(line.substr(version_end));

        if ( status_code_type.has_value() == false )
        {
            return http::start_line{ method(), version.value() };
        }

        return http::start_line{ method(), version.value(), static_cast<http::status_code_type>(status_code_type.value()) };
    }

    [[nodiscard]] auto size() const
    {
        return m_buffer.size();
    }

    private:

    [[nodiscard]] auto raw() const -> std::string_view
    {
        return {m_buffer.data(), m_buffer.size()};
    }

    [[nodiscard]] auto status_line_string() const -> std::string_view
    {
        if ( auto line = raw(); line.empty() == false )
        {
            if ( auto position = line.find(k_crlf); position != std::string_view::npos )
            {
                return std::string_view{m_buffer.data(), position};
            }
        }

        return {};
    }

    [[nodiscard]] auto fields_string() const -> std::string_view
    {
        if ( auto line = raw(); line.empty() == false )
        {
            if ( auto position = line.find(k_crlf); position != std::string_view::npos )
            {
                return line.substr(position + 2);
            }
        }

        return {};
    }

    platform::pmr::vector<char>     m_buffer{};
    platform::pmr::memory_resource* m_memory{};
};

} // namespace sys::http::parse
