#pragma once

namespace sys::utils {

template <typename T, std::size_t N>
class basic_ringbuffer
{
    public:

    using type                   = basic_ringbuffer<T, N>;
    using container_type         = std::array<T, N>;
    using size_type              = std::size_t;
    using value_type             = T;
    using pointer                = T*;
    using reference              = T&;
    using const_reference        = const T&;

    using iterator               = ring_buffer_iterator<type>;
    using const_iterator         = ring_buffer_const_iterator<type>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    friend iterator;
    friend const_iterator;

    basic_ringbuffer() noexcept = default;

    auto clear() noexcept
    {
        m_size      = decltype(m_size){};
        m_front_idx = decltype(m_front_idx){};
    }

    [[nodiscard]] constexpr auto empty() const noexcept -> bool
    {
        return m_size == 0;
    }

    [[nodiscard]] constexpr auto full() const noexcept ->bool
    {
        return m_size == N;
    }

    [[nodiscard]] constexpr auto size() const noexcept
    {
        return m_size;
    }

    [[nodiscard]] constexpr auto capacity() const noexcept
    {
        return N;
    }

    [[nodiscard]] constexpr auto front() noexcept -> reference
    {
        return *begin();
    }

    [[nodiscard]] constexpr auto back() noexcept -> reference
    {
        return *(--end());
    }

    [[nodiscard]] constexpr auto front() const noexcept -> const_reference
    {
        return *begin();
    }

    [[nodiscard]] constexpr auto back() const noexcept -> const_reference
    {
        return *(--end());
    }

    [[nodiscard]] constexpr auto begin() noexcept ->iterator
    {
        return iterator{m_front_idx, this};
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
    {
        return const_iterator{m_front_idx, this};
    }

    [[nodiscard]] constexpr auto end() noexcept ->iterator
    {
        return iterator{size() + m_front_idx, this};
    }

    [[nodiscard]] constexpr auto end() const noexcept -> const_iterator
    {
        return const_iterator{size() + m_front_idx, this};
    }

    [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator
    {
        return begin();
    }

    [[nodiscard]] constexpr auto rbegin() noexcept -> reverse_iterator
    {
        return reverse_iterator{end()};
    }

    [[nodiscard]] constexpr auto rbegin() const noexcept -> const_reverse_iterator
    {
        return const_reverse_iterator{end()};
    }

    [[nodiscard]] constexpr auto crbegin() const noexcept -> const_reverse_iterator
    {
        return const_reverse_iterator{end()};
    }

    [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator
    {
        return end();
    }

    [[nodiscard]] constexpr auto rend() noexcept -> reverse_iterator
    {
        return reverse_iterator{begin()};
    }

    [[nodiscard]] constexpr auto rend() const noexcept -> const_reverse_iterator
    {
        return const_reverse_iterator{begin()};
    }

    [[nodiscard]] constexpr auto crend() const noexcept -> const_reverse_iterator
    {
        return const_reverse_iterator{begin()};
    }

    auto push_back(const T& value) noexcept
    {
        m_data[back_idx()] = value;
        increase_size();
    }

    auto push_back(T&& value) noexcept
    {
        m_data[back_idx()] = std::move(value);
        increase_size();
    }

    template <class... FromType>
    auto emplace_back(FromType&&... from_value) noexcept -> reference
    {
        const auto index = back_idx();
        m_data[index] = T(std::forward<FromType>(from_value)...);
        increase_size();

        return m_data[index];
    }

    auto pop_front() -> value_type
    {
        assert(m_size != 0);
        auto old_front_idx = m_front_idx;

        m_front_idx = (m_front_idx + 1) % N;
        --m_size;

        return std::move(m_data[old_front_idx]);
    }

    auto pop_back() -> value_type
    {
        assert(m_size != 0);
        --m_size;

        return std::move(m_data[back_idx()]);
    }

    void swap(sys::utils::basic_ringbuffer<T, N>& rhs) noexcept
    {
        std::swap(m_data,      rhs.m_data);
        std::swap(m_size,      rhs.m_size);
        std::swap(m_front_idx, rhs.m_front_idx);
    }

    private:

    [[nodiscard]] constexpr auto at(size_type i) noexcept -> reference
    {
        return m_data[i % N];
    }

    [[nodiscard]] constexpr auto at(size_type i) const noexcept -> const_reference
    {
        return m_data[i % N];
    }

    [[nodiscard]] constexpr auto back_idx() const noexcept -> size_type
    {
        return (m_front_idx + m_size) % N;
    }

    void increase_size() noexcept
    {
        if ( ++m_size > N )
        {
            m_size      = N;
            m_front_idx = (m_front_idx + 1) % N;
        }
    }

    container_type m_data{};
    size_type      m_size{};
    size_type      m_front_idx{};
};

template <typename T, std::size_t N> requires(std::is_trivially_destructible<T>::value)
using ringbuffer = sys::utils::basic_ringbuffer<T, N>;

} // namespace sys::utils

