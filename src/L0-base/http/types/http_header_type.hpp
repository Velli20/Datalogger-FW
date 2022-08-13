#pragma once

namespace sys::http {

struct header
{
    explicit header(platform::pmr::memory_resource* memory)

        : m_memory{memory}
        , m_fields{memory}
    {}

    [[nodiscard]] auto fields() -> platform::pmr::vector<http::field>&
    {
        return m_fields;
    }

    [[nodiscard]] auto fields() const -> std::span<const http::field>
    {
        return std::span<const http::field>{m_fields.data(), m_fields.size()};
    }

    platform::pmr::memory_resource*    m_memory{};
    platform::pmr::vector<http::field> m_fields{};
};

struct header_view
{
    constexpr header_view() = default;

    constexpr header_view(std::span<const http::field> fields)
        : m_fields{fields}
    {}

    [[nodiscard]] auto fields() const -> std::span<const http::field>
    {
        return m_fields;
    }

    std::span<const http::field> m_fields{};
};

} // namespace sys::http
