#pragma once

namespace sys::units::literals {

constexpr auto operator"" _b(unsigned long long x)
{
    return 1ULL * x;
}

constexpr auto operator"" _KiB(unsigned long long x)
{
    return 1024_b * x;
}

constexpr auto operator"" _MiB(unsigned long long x )
{
    return 1024_KiB * x;
}

constexpr auto operator"" _GiB(unsigned long long x )
{
    return 1024_MiB *  x;
}

constexpr auto operator"" _TiB(unsigned long long x )
{
    return 1024_GiB *  x;
}

using Hz  = std::chrono::duration<double>;
using KHz = std::chrono::duration<double, std::kilo>;
using MHz = std::chrono::duration<double, std::mega>;
using GHz = std::chrono::duration<double, std::giga>;

constexpr Hz operator"" _hz(long double d)
{
    return Hz(d);
}

constexpr KHz operator"" _khz(long double d)
{
    return KHz(d);
}

constexpr MHz operator"" _mhz(long double d)
{
    return MHz(d);
}

constexpr GHz operator"" _ghz(long double d)
{
    return GHz(d);
}

constexpr std::size_t operator"" _Hz(unsigned long long int value)
{
    return value;
}

constexpr std::size_t operator"" _kHz(unsigned long long int value)
{
    return value * 1000_Hz;
}

constexpr std::size_t operator"" _MHz(unsigned long long int value)
{
    return value * 1000_kHz;
}

} // namespace sys::units::literals

