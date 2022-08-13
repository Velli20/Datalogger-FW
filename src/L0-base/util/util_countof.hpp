#pragma once

namespace sys::util {

template <typename T, const std::size_t N>
constexpr auto count_of(T(&&a)[N]) -> std::size_t
{
    return sizeof(a) / sizeof(a[0U]);
}

template <typename T, const std::size_t N>
constexpr auto count_of(T(&a)[N]) -> std::size_t
{
    return sizeof(a) / sizeof(a[0U]);
}

static_assert(sys::util::count_of({'a', 'b', 'c'})                      == 3);
static_assert(sys::util::count_of({std::uint32_t{0}, std::uint32_t{0}}) == 2);

} // namespace sys::util
