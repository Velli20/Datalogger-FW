#pragma once

namespace sys::log {

template <class T>
constexpr auto operator<<(
    log::formatting_stream<T>& os,
    const std::string_view     s
) noexcept -> log::formatting_stream<T>&
{
    if ( s.empty() )
    {
        os.out(std::string_view{"(empty string view)"});
    }

    else
    {
        os.out(s);
    }

    return os;
}

} // namespace sys::log
