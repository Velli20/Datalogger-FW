#pragma once

namespace sys::utils {

template <typename T, bool Const>
class ring_iterator
{
    private:

    using container_type    = typename std::conditional_t<Const, const T*, T*>;

    public:

    using base_type         = typename T::value_type;
    using pointer           = std::conditional_t<Const, std::add_pointer_t<base_type const>,          std::add_pointer_t<base_type>>;
    using reference         = std::conditional_t<Const, std::add_lvalue_reference_t<base_type const>, std::add_lvalue_reference_t<base_type>>;

    using type              = ring_iterator<T, Const>;
    using value_type        = typename T::value_type;
    using difference_type   = std::ptrdiff_t;


    using iterator_category = std::random_access_iterator_tag;
    using size_type         = typename T::size_type;

    constexpr ring_iterator(
        size_type      idx,
        container_type rv
    ) noexcept
        : m_idx{idx}
        , m_rv{rv}
    {}

    [[nodiscard]] constexpr bool operator==  (const type&) const = default;
    [[nodiscard]] constexpr auto operator<=> (const type&) const = default;

    [[nodiscard]] constexpr auto operator*() const noexcept -> reference
    {
          return m_rv->at(m_idx);
    }

    [[nodiscard]] constexpr auto operator->() const noexcept -> pointer
    {
        return std::pointer_traits<pointer>::pointer_to(operator*());
    }

    [[maybe_unused]] constexpr auto operator++() noexcept -> ring_iterator&
    {
        ++m_idx;
        return *this;
    }

    [[maybe_unused]] constexpr auto operator--() noexcept -> ring_iterator&
    {
        --m_idx;
        return *this;
    }

    [[maybe_unused]] constexpr auto operator++(int) noexcept -> ring_iterator
    {
        auto temporary_iterator{ *this };

        operator++();

        return temporary_iterator;
    }

    [[maybe_unused]] constexpr auto operator--(int) noexcept -> ring_iterator
    {
        auto temporary_iterator{ *this };

        operator--();

        return temporary_iterator;
    }

    [[maybe_unused]] constexpr auto operator+=(std::integral auto delta) noexcept -> ring_iterator&
    {
        this->m_idx += delta;

        return *this;
    }

    [[maybe_unused]] constexpr auto operator-=(std::integral auto delta) noexcept -> ring_iterator&
    {
        this->m_idx -= delta;

        return *this;
    }

    [[nodiscard]] constexpr auto operator-(const ring_iterator& rhs) const noexcept -> difference_type
    {
        return static_cast<difference_type>(this->m_idx) - static_cast<difference_type>(rhs.m_idx);
    }

    private:

    friend T;
    friend class ring_iterator<T, !Const>;

    size_type      m_idx{};
    container_type m_rv{};
};

template <typename T>
using ring_buffer_iterator = ring_iterator<T, false>;

template <typename T>
using ring_buffer_const_iterator = ring_iterator<const T, true>;

} // namespace sys::utils

template <typename T>
class std::iterator_traits<sys::utils::ring_buffer_iterator<T>>
{
    public:

    using difference_type   = typename std::make_signed<typename T::size_type>::type;
    using value_type        = typename T::value_type;
    using pointer           = value_type*;
    using reference         = value_type&;
    using iterator_category = std::random_access_iterator_tag;
};

template <typename T>
class std::iterator_traits<sys::utils::ring_buffer_const_iterator<T>>
{
    public:

    using difference_type   = typename std::make_signed<typename T::size_type>::type;
    using value_type        = const typename T::value_type;
    using pointer           = const value_type*;
    using reference         = const value_type&;
    using iterator_category = std::random_access_iterator_tag;
};
