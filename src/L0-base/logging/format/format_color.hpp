#pragma once

namespace sys::log {

template <class T, typename U> requires(std::is_base_of<format::color, U>::value)
constexpr auto operator<<(log::formatting_stream<T>& os, U) noexcept -> log::formatting_stream<T>&
{
    [[maybe_unused]] constexpr auto reset   = std::string_view{"\033[0m"};
    [[maybe_unused]] constexpr auto red     = std::string_view{"\033[0;31m"};
    [[maybe_unused]] constexpr auto green   = std::string_view{"\033[0;32m"};
    [[maybe_unused]] constexpr auto yellow  = std::string_view{"\033[0;33m"};
    [[maybe_unused]] constexpr auto blue    = std::string_view{"\033[0;34m"};
    [[maybe_unused]] constexpr auto magenta = std::string_view{"\033[0;35m"};

    if constexpr ( std::is_same<U, format::color_red>::value )
    {
        os.out(red);
    }

    if constexpr ( std::is_same<U, format::color_yellow>::value )
    {
        os.out(yellow);
    }

    if constexpr ( std::is_same<U, format::color_blue>::value )
    {
        os.out(blue.data());
    }

    if constexpr ( std::is_same<U, format::color_magenta>::value )
    {
        os.out(magenta.data());
    }

    if constexpr ( std::is_same<U, format::color_reset>::value )
    {
        os.out(reset.data());
    }

    return os;
}

} // namespace sys::log
