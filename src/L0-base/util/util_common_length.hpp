#pragma once

namespace sys::util {

[[nodiscard]] constexpr auto find_common_length(
    const char* str_1,
    std::size_t max_1,
    const char* str_2,
    std::size_t max_2
) noexcept -> std::size_t
{
    if ( str_1 == nullptr || str_2 == nullptr )
        return {};

    std::size_t i{};

    while ( i != max_1 && i != max_2 )
    {
        if ( str_1[i] != str_2[i] )
        {
            return i;
        }

        if ( str_1[i + 1] == '\0' && (i == (max_1 - 1)) )
        {
            return i;
        }

        ++i;
    }

    return i;
}

} // namespace sys::util
