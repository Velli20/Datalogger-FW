#pragma once

namespace sys::io {

struct input_sink_interface
{
    constexpr virtual ~input_sink_interface() noexcept = default;

    [[maybe_unused]] constexpr virtual auto operator>>(std::span<std::byte> buffer) -> input_sink_interface& = 0;
    [[maybe_unused]] constexpr virtual auto operator>>(std::byte& b)                -> input_sink_interface& = 0;
};

struct input_sink_iterator
{
    using difference_type   = std::size_t;
    using value_type        = std::byte;
    using iterator_category    = std::input_iterator_tag;

    input_sink_iterator(
        io::input_sink_interface& in,
        std::size_t               available,
        std::size_t               consumed = std::size_t{}
    )
        : m_in{in}
        , m_available{available}
        , m_consumed{consumed}
        {}

    [[maybe_unused]] constexpr decltype(auto) operator++(int)
    {
        ++m_consumed;
        return *this;
    }

    [[maybe_unused]] constexpr decltype(auto) operator++()
    {
        ++m_consumed;
        return *this;
    }

    [[maybe_unused]] constexpr decltype(auto) operator+=(difference_type n)
    {
        m_consumed += n;
        return *this;
    }

    [[nodiscard]] constexpr decltype(auto) operator*()
    {
        value_type c{};
        m_in >> c;
        return c;
    }

    [[nodiscard]] bool operator==(const input_sink_iterator& rhs) const
    {
        return m_consumed == rhs.m_consumed;
    }

    [[nodiscard]] bool operator!=(const input_sink_iterator& rhs) const
    {
        return m_consumed != rhs.m_consumed;
    }

    [[nodiscard]] auto available() const
    {
        return m_available;
    }

    io::input_sink_interface& m_in;
    std::size_t               m_available{};
    std::size_t               m_consumed{};
};

} // namespace sys::io

