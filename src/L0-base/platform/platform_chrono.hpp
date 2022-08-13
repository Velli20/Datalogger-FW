#pragma once

namespace platform::chrono {

struct high_resolution_clock
{
    using rep        = std::uint32_t;
    using period     = std::milli;
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<high_resolution_clock>;

    static constexpr bool is_steady = false;

    static time_point now() noexcept;
};

struct system_clock
{
    using duration   = std::chrono::seconds;
    using rep        = duration::rep;
    using period     = duration::period;
    using time_point = std::chrono::time_point<system_clock>;

    static constexpr bool is_steady = false;

    static time_point now() noexcept;

    static std::time_t to_time_t(const time_point& t) noexcept
    {
        return std::chrono::duration_cast<std::chrono::duration<std::time_t>>(t.time_since_epoch()).count();
    }

    static time_point from_time_t(std::time_t t) noexcept
    {
        return time_point{std::chrono::duration_cast<duration>(std::chrono::duration<std::time_t>{t})};
    }
};

} // namespace platform::chrono
