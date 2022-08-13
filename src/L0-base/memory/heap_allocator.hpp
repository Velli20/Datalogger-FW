
#include "detail/allocator_traits.hpp"
#include "detail/memory_utils.hpp"
#include "detail/block_header_type.hpp"
#include "detail/block_type.hpp"
#include "detail/block_freelist.hpp"

#pragma once

namespace sys {

struct heap_allocator
{
    using value_type            = std::byte;
    using pointer_type          = value_type*;
    using difference_type       = typename std::pointer_traits<pointer_type>::difference_type;
    using size_type             = std::make_unsigned_t<difference_type>;
    using is_always_equal       = std::true_type; //!\brief Do any two allocators of the same  type always compare equal?
    using alignment_offset_type = std::uint8_t;

    private:

    static constexpr auto aligned_ptr = [](
        void*       pointer,
        std::size_t alignment,
        bool        down = false
    ) -> pointer_type
    {
        auto address = reinterpret_cast<std::uintptr_t>(pointer);

        if ( auto rem = address % alignment; std::cmp_equal(rem, decltype(rem){}) == false )
        {
            if ( down )
            {
                address -= rem;
            }

            else
            {
                address += alignment - rem;
            }

            return std::bit_cast<pointer_type>(address);
        }

        return std::bit_cast<pointer_type>(pointer);
    };

    static constexpr auto alignment_padding = [](
        const pointer_type pointer,
        std::size_t        alignment) -> std::size_t
    {
        if ( auto remaining = std::bit_cast<std::uintptr_t>(pointer) % alignment; remaining != 0 )
        {
            return alignment - remaining;
        }

        return {};
    };

    public:

    explicit heap_allocator(
        pointer_type begin,
        pointer_type end
    ) noexcept
        : m_memory
        {
            aligned_ptr(begin, alignof(void*), false),

            static_cast<std::size_t>(
                aligned_ptr(end,   alignof(void*), true) -
                aligned_ptr(begin, alignof(void*), false)
            )
        }
        , m_list{m_memory}
    {
        assert((detail::align_of(reinterpret_cast<void*>(m_memory.data())) >= alignof(void*)) && "Alignment");
    }

    [[nodiscard]] constexpr auto allocate(std::size_t size)  noexcept -> pointer_type
    {
        size += alignof(void*) - (size % alignof(void*));

        if ( auto header = m_list.request(size); header != nullptr )
        {
            return std::launder(header->data());
        }

        return nullptr;
    }


    [[nodiscard]] constexpr auto allocate(std::size_t size, std::size_t alignment) noexcept -> pointer_type
    {
        if ( std::has_single_bit(alignment) == false )
        {
            assert(false && "Alignment must be a power of 2");
            return nullptr;
        }

        if ( std::cmp_less(alignment, alignof(void*)) )
        {
            alignment = alignof(void*);
        }

        // Add the amount adjusted for the alignment.

        size += alignment;

        if ( auto* pointer = allocate(size); pointer != nullptr )
        {
            auto offset = alignment_padding(pointer, alignment);
            assert(std::cmp_less(offset, std::numeric_limits<alignment_offset_type>::max()));

            auto* point = pointer + offset;

            // Adjust the aligned pointer backward and store the alignment offset.

            point -= 1;
            detail::store_unaligned(point, static_cast<alignment_offset_type>(offset));
            point += 1;

            return std::launder(point);
        }

        return nullptr;
    }

    constexpr void deallocate(void* pointer, std::size_t align)  noexcept
    {
        assert(std::has_single_bit(align) && "Alignment must be a power of 2");
        assert(contains(pointer));

        if ( pointer != nullptr )
        {
            // Adjust the aligned pointer backward and load the alignment offset.

            const auto offset = [&]()
            {
                auto* point = static_cast<alignment_offset_type*>(pointer);
                point--;

                return detail::load_unaligned<alignment_offset_type>(point);
            }();

            // Adjust the pointer by the alignment offset value to get real aclloation pointer.

            auto* real_pointer = static_cast<pointer_type>(pointer) - offset;

            // Do deallocate.

            deallocate(real_pointer);
        }
    }

    constexpr void deallocate(void* pointer)  noexcept
    {
        assert(contains(pointer));

        if ( auto* header = detail::block_header::from_data(detail::aliased(pointer)); header != nullptr )
        {
            m_list.release(std::launder(header));
        }
    }

    [[nodiscard]] constexpr auto allocation_size(const void* pointer) const noexcept -> std::size_t
    {
        if ( auto* h = detail::block_header::from_data(detail::aliased(pointer)); h != nullptr )
        {
           return m_list.allocation_size(h);
        }

        assert(false && "Invalid block");
        return {};
    }

    [[nodiscard]] constexpr auto contains(const void* address) const noexcept -> bool
    {
        return std::addressof(m_memory.front()) <= address && address < std::addressof(m_memory.back());
    }

    [[nodiscard]] constexpr auto allocated() const noexcept
    {
        return m_list.m_allocated;
    }

    [[nodiscard]] constexpr auto size() const noexcept
    {
        return m_memory.size();
    }

    private:

    std::span<std::byte> m_memory{};
    detail::freelist     m_list;
};

} // namespace sys
