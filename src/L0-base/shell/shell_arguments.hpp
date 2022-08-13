#pragma once

namespace sys::shell {

struct arguments
{
    using T               = std::string_view;
    using value_type      = T;
    using values_type     = std::span<value_type>;
    using size_type       = std::size_t;
    using iterator        = typename values_type::iterator;
    using difference_type = std::ptrdiff_t;

    constexpr arguments() = default;

    explicit constexpr arguments(values_type args)
        : m_args{std::move(args)}
    {}

    [[nodiscard]] constexpr auto empty() const -> bool
    {
        return m_args.empty();
    }

    [[nodiscard]] constexpr auto program_name() const -> value_type
    {
        if ( empty() == false )
        {
            return m_args.front();
        }

        return {};
    }

    [[nodiscard]] constexpr auto size() const -> size_type
    {
        if ( empty() == false )
        {
            return m_args.size() - 1;
        }

        return {};
    }

    [[nodiscard]] constexpr auto cbegin() const -> iterator
    {
        if ( size() < 1 )
        {
            return end();
        }

        return m_args.subspan(1).begin();
    }

    [[nodiscard]] constexpr auto begin() const -> iterator
    {
        return cbegin();
    }

    [[nodiscard]] constexpr auto end() const -> iterator
    {
        return m_args.end();
    }

    [[nodiscard]] constexpr auto cend() const -> iterator
    {
        return m_args.end();
    }

    [[nodiscard]] constexpr auto operator[](size_type n) const -> value_type
    {
        if ( size() > 0 && n < size() )
        {
            return m_args[n + 1];
        }

        return {};
    }

    constexpr auto clear()
    {
        m_args = values_type{};
    }

    private:

    values_type m_args{};
};

} // namespace sys::shell
