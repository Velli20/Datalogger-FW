#pragma once

namespace sys::util {

template <typename FunctionType> requires (std::is_invocable_r_v<bool, FunctionType>)
static inline auto busy_wait(
    const std::chrono::duration<std::uint32_t, std::milli> timeout,
    FunctionType                                           until
) noexcept
{
    if ( timeout == std::chrono::milliseconds::zero() )
    {
        return std::invoke(until);
    }

    const auto expire = platform::chrono::high_resolution_clock::now() + timeout;

    const auto check_timeout = [=]() -> bool
    {
        if ( platform::chrono::high_resolution_clock::now() >= expire )
        {
            return true;
        }

        return false;
    };

    do
    {
        if ( std::invoke(until) == true )
        {
            return true;
        }

        if ( check_timeout() == true )
        {
            return false;
        }
    }
    while ( true );

    return false;
}

} // namespace sys::util
