#pragma once

namespace sys {

constexpr inline auto debug_assert(
    const bool                  condition,
    const std::source_location location = std::source_location::current()
) noexcept
{
    static_cast<void>(condition);
    static_cast<void>(location);

    if constexpr ( SYS_RELEASE_MODE )
    {
        return;
    }

    if ( not condition )
    {
        __assert_func(location.file_name(), location.line(), location.function_name(), "");
    }
}

} // namespace sys
