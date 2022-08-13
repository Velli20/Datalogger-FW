#pragma once

namespace sys::detail {

[[nodiscard]] inline auto align_of(const void* pointer) noexcept
{
    const auto address = reinterpret_cast<std::uintptr_t>(pointer);

    return static_cast<std::size_t>(address & ((~address) + 1));
}

template<typename T>
[[nodiscard]] constexpr bool is_aligned(T *p)
{
    return (reinterpret_cast<ptrdiff_t>(p) % std::alignment_of<T>::value) == 0;
}

[[nodiscard]] constexpr bool equal_ptr(auto* p1, auto* p2) noexcept
{
    return static_cast<void*>(p1) == static_cast<void*>(p2);
}


[[nodiscard]] constexpr auto aliased(const void* pointer) noexcept -> const std::byte*
{
    union
    {
        const void*      m_pointer;
        const std::byte* m_data;
    }
    aliased_un { .m_pointer = pointer };

    return aliased_un.m_data;
}

[[nodiscard]] constexpr auto aliased(void* pointer) noexcept -> std::byte*
{
    union
    {
        void*      m_pointer;
        std::byte* m_data;
    }
    aliased_un { .m_pointer = pointer };

    return aliased_un.m_data;
}

[[nodiscard]] constexpr auto is_power_of_two(std::size_t alignment) noexcept -> bool
{
    return (alignment != 0u && !(alignment & (alignment - 1u)));
}

template<typename T> requires(std::is_trivially_copyable<T>::value)
constexpr auto store_unaligned(void* pointer, const T& value)
{
    if ( std::is_constant_evaluated() == false )
    {
        assert((align_of(pointer) >= alignof(T)) && "Alignment");
    }

    union
    {
        void* m_pointer;
        T*    m_data;
    }
    aliased_un { .m_pointer = pointer };

    *(aliased_un.m_data) = value;
}

template<typename T> requires(std::is_trivially_copyable<T>::value)
[[nodiscard]] constexpr auto load_unaligned(const void* pointer)
{
    if ( std::is_constant_evaluated() == false )
    {
        assert((align_of(pointer) >= alignof(T)) && "Alignment");
    }

    union
    {
        const void* m_pointer;
        const T*    m_data;
    }
    aliased_un { .m_pointer = pointer };

    return *(aliased_un.m_data);
}

} // namespace sys::detail
