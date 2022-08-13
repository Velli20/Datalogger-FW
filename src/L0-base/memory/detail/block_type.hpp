#pragma once

namespace sys::detail {

struct /* alignas(alignof(void*)) */ block
{
    constexpr block() = default;

    constexpr explicit block(std::size_t size)
    {
        m_header.size(size);

        m_next_free     = nullptr;
        m_previous_free = nullptr;
    }

    constexpr explicit block(const block_header& header, detail::launder)
        : m_header{header, detail::launder{}}
    {}

    block(block&&)                  = delete;
    block(const block&)             = delete;
    block & operator=(block&&)      = delete;
    block & operator=(const block&) = delete;

    [[nodiscard]] static constexpr auto assume_free(block_header* p) -> block*
    {
        union
        {
            block*        m_block;
            block_header* m_header;
        }
        aliased { .m_header = p };

        return aliased.m_block;
    }

    [[nodiscard]] constexpr auto address() noexcept -> std::byte*
    {
        union
        {
            block*     m_block;
            std::byte* m_bytes;
        }
        aliased { .m_block = this };

        return aliased.m_bytes;
    }

    [[nodiscard]] constexpr auto address() const noexcept -> const std::byte*
    {
        union
        {
            const block*     m_block;
            const std::byte* m_bytes;
        }
        aliased { .m_block = this };

        return aliased.m_bytes;
    }

    [[nodiscard]] constexpr auto size() const noexcept
    {
        return m_header.size();
    }

    constexpr auto size(std::size_t size) noexcept
    {
        m_header.size(size);
    }

    [[nodiscard]] constexpr auto next() const noexcept -> block_header*
    {
        return m_header.m_next;
    }

    [[nodiscard]] constexpr auto next_free() const noexcept -> block*
    {
        return m_next_free;
    }

    [[nodiscard]] constexpr auto previous() const noexcept -> block_header*
    {
        return m_header.m_prev;
    }

    [[nodiscard]] constexpr auto previous_free() const noexcept -> block*
    {
        return m_previous_free;
    }

    constexpr void append_to_list(block* p) noexcept
    {
        append_to_list(std::addressof(p->m_header));
    }

    constexpr void append_to_list(block_header* p) noexcept
    {
        // Insert the block.

        if ( m_header.m_next )
        {
            m_header.m_next->m_prev = p;
        }

        p->m_next = m_header.m_next;
        p->m_prev = std::addressof(m_header);
        m_header.m_next = p;
    }

    constexpr void append_to_freelist(block* p) noexcept
    {
        if ( m_next_free )
        {
            m_next_free->m_previous_free = p;
        }

        p->m_next_free     = m_next_free;
        p->m_previous_free = this;

        m_next_free = p;

        p->set_free();
        p->merge();
    }

    constexpr void prepend_to_freelist(block* p) noexcept
    {
        if ( m_previous_free )
        {
            m_previous_free->m_next_free = p;
        }

        p->m_previous_free = m_previous_free;
        p->m_next_free     = this;

        m_previous_free = p;

        p->set_free();
        p->merge();
    }

    [[nodiscard]] constexpr auto unlink_from_list() noexcept -> block*
    {
        if ( m_header.m_prev )
        {
            m_header.m_prev->m_next = m_header.m_next;
        }

        if ( m_header.m_next )
        {
            m_header.m_next->m_prev = m_header.m_prev;
        }

        return this;
    }

    /* [[nodiscard]]  */constexpr auto unlink_from_freelist() noexcept -> block*
    {
        if ( m_previous_free )
        {
            m_previous_free->m_next_free = m_next_free;
        }

        if ( m_next_free )
        {
            m_next_free->m_previous_free = m_previous_free;
        }

        set_allocated();
        return this;
    }

    constexpr auto split(std::size_t size) noexcept -> block*
    {
        // Create new block.

        union
        {
            block*     m_block;
            std::byte* m_bytes;
        }
        aliased { .m_bytes = address() + size };

        aliased.m_block->size(this->size() - size);

        //block* tail = ::new (address() + size) block{m_header.m_size - size};

        append_to_list(aliased.m_block);
        append_to_freelist(aliased.m_block);

        // Shrink current block.

        m_header.m_size = size;

        return aliased.m_block;
    }

    constexpr void merge_next() noexcept
    {
        // Increase current size.

        m_header.m_size += m_next_free->m_header.m_size;

        // Remove next from free list.

        (void) m_next_free->unlink_from_freelist();

        // Remove block from complete list.

        (void) assume_free(m_header.m_next)->unlink_from_list();
    }

    constexpr void merge() noexcept
    {
        if ( m_next_free != nullptr && address() + size() == m_next_free->address() )
        {
            merge_next();
        }

        if ( m_previous_free == nullptr )
        {
            return;
        }

        if ( m_previous_free->address() + m_previous_free->size()== address() )
        {
            m_previous_free->merge_next();
        }
    }

    [[nodiscard]] constexpr auto is_free() const noexcept
    {
        return m_header.is_free();
    }

    constexpr void set_free() noexcept
    {
        m_header.mark_free();
    }

    constexpr void set_allocated() noexcept
    {
        m_header.mark_used();
    }

    block_header m_header;
    block*       m_next_free{};
    block*       m_previous_free{};
};

} // namespace sys::detail
