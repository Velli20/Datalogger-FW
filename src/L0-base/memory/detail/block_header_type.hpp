#pragma once

namespace sys::detail {

struct launder{};

struct /* alignas(alignof(void*)) */ block_header
{
    private:

    static constexpr std::size_t k_max_size_bits  = 30u;
    static constexpr std::size_t k_max_block_size = (1u << k_max_size_bits);

    public:

    constexpr block_header() = default;

    constexpr block_header(const block_header& other, detail::launder)
        : block_header{other}
    {
        if ( m_next )
        {
            m_next->m_prev = std::launder(m_next->m_prev);
        }

        if ( m_prev )
        {
            m_prev->m_next = std::launder(m_prev->m_next);
        }
    }

    [[nodiscard]] constexpr auto is_free() const noexcept
    {
        return m_used == 0u;
    }

    [[nodiscard]] constexpr auto mark_used() noexcept
    {
        assert(is_free() == true);

        m_used = 1u;
    }

    [[nodiscard]] constexpr auto mark_free() noexcept
    {
        //assert(is_free() == false);

        m_used = 0u;
    }

    [[nodiscard]] constexpr auto size() const noexcept
    {
        return m_size;
    }

    constexpr auto size(std::size_t size) noexcept
    {
        m_size = size;
    }

    [[nodiscard]] constexpr auto data_size() const noexcept
    {
        return size() - sizeof(*this);
    }

    [[nodiscard]] static constexpr auto from_data(std::byte* data) noexcept
    {
        union
        {
            block_header*    m_header;
            std::byte* m_bytes;
        }
        aliased { .m_bytes = data - sizeof(block_header) };

        return std::launder(aliased.m_header);
    }

    [[nodiscard]] static constexpr auto from_data(const std::byte* data) noexcept
    {
        union
        {
            const block_header* m_header;
            const std::byte*    m_bytes;
        }
        aliased { .m_bytes = data - sizeof(block_header) };

        return std::launder(aliased.m_header);
    }

    [[nodiscard]] constexpr auto data() noexcept -> std::byte*
    {
        union
        {
            block_header* m_header;
            std::byte*    m_bytes;
        }
        aliased { .m_header = this };

        auto address = aliased.m_bytes + sizeof(*this);

        return std::launder(address);
    }

    [[nodiscard]] constexpr auto next() const -> block_header*
    {
        return m_next;
    }

    [[nodiscard]] constexpr auto previous() const -> block_header*
    {
        return m_prev;
    }

    constexpr auto previous(block_header* h)
    {
        m_prev = h;
    }

    constexpr auto next(block_header* h)
    {
        m_next = h;
    }

    //private:

    block_header*     m_next{};
    block_header*     m_prev{};
    std::size_t m_size : 30 {};
    std::size_t m_used : 1  {};
};

} // namespace sys::detail
