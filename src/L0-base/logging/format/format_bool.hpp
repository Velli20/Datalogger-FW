#pragma once

namespace sys::log {

template <class T, typename U> requires(std::is_same<U, bool>::value)
constexpr auto operator<<(log::formatting_stream<T>& os, U arg) noexcept -> log::formatting_stream<T>&
{
    constexpr auto k_true  = std::string_view{"true"};
    constexpr auto k_false = std::string_view{"false"};

    if ( arg == true )
    {
        os.out(k_true);
    }

    else
    {
        os.out(k_false);
    }

    return os;
}

} // namespace sys::log
