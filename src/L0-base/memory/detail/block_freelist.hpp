#pragma once

namespace sys::detail {

struct freelist
{
    explicit freelist(const std::span<std::byte>& memory) noexcept
        : m_first_free{::new (memory.data()) block{memory.size()}}
        , m_first{m_first_free}
    {}

    freelist(freelist&& other) noexcept
        : m_first_free{std::exchange(other.m_first_free, nullptr)}
        , m_first{std::exchange(other.m_first, nullptr)}
        , m_allocated{std::exchange(other.m_allocated, std::size_t{})}
    {}

    auto operator=(freelist&& other) noexcept -> freelist&
    {
        m_first_free = std::exchange(other.m_first_free, nullptr);
        m_first      = std::exchange(other.m_first, nullptr);
        m_allocated  = std::exchange(other.m_allocated, std::size_t{});

        return *this;
    }

    freelist(const freelist&)            = delete;
    freelist& operator=(const freelist&) = delete;

    [[nodiscard]] constexpr auto request(std::size_t size) noexcept -> block_header*
    {
        // Block sizes include the header.

        size += sizeof(block_header);

        // Make sure we only allocate properly aligned data.

        size += ((size + alignof(detail::block) - 1) / alignof(detail::block) * alignof(detail::block)) - size;

        for ( auto point = m_first_free; point != nullptr; point = point->next_free() )
        {
            // If not enough size, continue.

            if ( std::cmp_less(point->size(), size) )
            {
                continue;
            }

            // If there is leftover space for a node, split the current node.

            if ( std::cmp_greater_equal(point->size() - size, sizeof(block)) )
            {
                point->split(size);
            }

            // Unlink the block from the freelist.

            point->unlink_from_freelist();

            // If we just allocated the first node, update the first free node.

            if ( detail::equal_ptr(point, m_first_free) )
            {
                m_first_free = point->next_free();
            }

            // Count allocation.

            m_allocated += point->size();

            union
            {
                block*        m_block;
                block_header* m_header;
            }
            aliased { .m_block = point };

            if ( std::is_constant_evaluated() == false )
            {
                assert((align_of(aliased.m_header) >= alignof(detail::block)) && "Alignment");
            }

            // replace the full node with a header.

            return std::construct_at(aliased.m_header, point->m_header, detail::launder{});
        }

        return nullptr;
    }

    constexpr void release(block_header* h) noexcept
    {
        if ( std::is_constant_evaluated() == false )
        {
            assert((align_of(h) >= alignof(detail::block)) && "Alignment");
        }

        // Recreate the full node.

        auto* node = [&]()
        {
            union
            {
                block*        m_block;
                block_header* m_header;
            }
            aliased { .m_header = h };

            return aliased.m_block;
        }();

        // Count deallocation.

        m_allocated -= node->size();

        // Find first free node.

        for ( auto p = node->previous(); p; p = p->m_prev )
        {
            if ( !p->is_free() )
            {
                continue;
            }

            block::assume_free(p)->append_to_freelist(node);
            return;
        }

        // If this is not the last node, prepend to first.

        if ( m_first_free )
        {
            m_first_free->prepend_to_freelist(node);
        }

        else
        {
            node->m_next_free     = nullptr;
            node->m_previous_free = nullptr;
        }

        m_first_free = node;
    }

    [[nodiscard]] constexpr auto allocation_size(const block_header* header) const noexcept
    {
        return header->data_size();
    }

    block*      m_first_free{};
    block*      m_first{};
    std::size_t m_allocated{};
};

} // namespace sys::detail
