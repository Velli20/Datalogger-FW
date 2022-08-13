#pragma once

namespace sys::log::format {

struct color{};

struct color_red     : public color {};
struct color_yellow  : public color {};
struct color_blue    : public color {};
struct color_magenta : public color {};
struct color_reset   : public color {};

[[maybe_unused]] constexpr auto hex         = format_flag::k_hex_format;
[[maybe_unused]] constexpr auto bin         = format_flag::k_bin_format;
[[maybe_unused]] constexpr auto dec         = format_flag::k_decimal_format;
[[maybe_unused]] constexpr auto showbase    = format_flag::kShowBase;
[[maybe_unused]] constexpr auto noshowbase  = format_flag::kNoShowBase;
[[maybe_unused]] constexpr auto uppercase   = format_flag::kUpperCase;
[[maybe_unused]] constexpr auto nouppercase = format_flag::kNoUpperCase;

[[maybe_unused]] constexpr auto red         = color_red{};
[[maybe_unused]] constexpr auto yellow      = color_yellow{};
[[maybe_unused]] constexpr auto blue        = color_blue{};
[[maybe_unused]] constexpr auto magenta     = color_magenta{};
[[maybe_unused]] constexpr auto reset_color = color_reset{};

} // namespace sys
