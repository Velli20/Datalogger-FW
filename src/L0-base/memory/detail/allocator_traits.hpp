#pragma once

namespace sys::detail {

template <typename T = std::byte>
struct allocator_traits
{
    using value_type                             = T;
    using pointer                                = value_type*;
    using const_pointer                          = const pointer;
    using void_pointer                           = void*;
    using const_void_pointer                     = const void_pointer;
    using reference                              = typename std::add_lvalue_reference<value_type>::type;
    using const_reference                        = typename std::add_lvalue_reference<const value_type>::type;
    using size_type                              = std::size_t;
    using difference_type                        = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal                        = std::true_type;
    using alignment_offset_type                  = std::uint8_t;
};

} // namespace sys::detail
