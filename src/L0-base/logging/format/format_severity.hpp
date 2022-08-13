#pragma once

namespace sys::log {

template <class T>
constexpr auto operator<<(
    log::formatting_stream<T>& os,
    severity                   arg
) noexcept -> log::formatting_stream<T>&
{
    using namespace std::string_view_literals;

    constexpr auto k_debug_str   = "[DEBUG]   | "sv;
    constexpr auto k_info_str    = "[INFO]    | "sv;
    constexpr auto k_warning_str = "[WARNING] | "sv;
    constexpr auto k_error_str   = "[ERROR]   | "sv;

    switch ( arg )
    {
        case severity::k_debug:
            os.out(k_debug_str);
            break;

        case severity::k_info:
            os.out(k_info_str);
            break;

        case severity::k_warning:
            os.out(k_warning_str);
            break;

        case severity::k_error:
            os.out(k_error_str);
            break;
    }

    return os;
}

} // namespace sys::log
