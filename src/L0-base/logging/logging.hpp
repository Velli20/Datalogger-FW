#pragma once

#include <logging/format/format_flag.hpp>
#include <logging/format/format.hpp>

#include <logging/formatting_stream.hpp>

#include <logging/format/format_string_view.hpp>
#include <logging/format/format_bool.hpp>
#include <logging/format/format_std_time.hpp>
#include <logging/format/format_integer.hpp>
#include <logging/format/format_color.hpp>
#include <logging/format/format_severity.hpp>
#include <logging/hexdump.hpp>

namespace sys::log {

constexpr auto debug(bool enable = true)
{
    auto os = log::formatting_stream{io::stdio_stream_sink{enable}};

    return os << log::format::yellow << severity::k_debug << log::format::reset_color;
}

constexpr auto info()
{
    auto os = log::formatting_stream{io::stdio_stream_sink{}};

    return os << log::format::magenta << severity::k_info << log::format::reset_color;
}

constexpr auto error()
{
    auto os = log::formatting_stream{io::stdio_stream_sink{}};

    return os << log::format::red << severity::k_error << log::format::reset_color;
}

} // namespace sys::log
