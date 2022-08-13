#pragma once

namespace sys::this_thread {

namespace detail {

template <class Rep, class Period>
constexpr auto ticks(const std::chrono::duration<Rep, Period>& time_point) -> os::tick_type
{
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(time_point);
    if ( millis == std::chrono::milliseconds::max() )
    {
        return portMAX_DELAY;
    }

    return (millis.count() / portTICK_PERIOD_MS);
}

} // namespace detail

template <class Rep, class Period>
inline void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration)
{
    const auto tick_count = detail::ticks(sleep_duration);
    vTaskDelay(tick_count);
}

inline auto yield()
{
    taskYIELD();
}

template <typename F>
inline auto yield_while(F&& condition)
{
    while ( condition() )
    {
        yield();
    }
}

template <typename P>
inline auto yield_until(const P& time_point)
{
    yield_while([&time_point] { return platform::chrono::high_resolution_clock::now() < time_point; });
}

inline auto yield_for(const typename platform::chrono::high_resolution_clock::duration& duration)
{
    yield_until(platform::chrono::high_resolution_clock::now() + duration);
};

// Combination of yield_while() and yield_until(). Returns true on timeout.
template <typename F, typename P>
[[nodiscard]] inline auto yield_while_until(F&& condition, const P& time_point)
{
    bool c{};

    yield_while([&] { return (c = condition()) and platform::chrono::high_resolution_clock::now() < time_point; });

    return c;
};

// Combination of yield_while() and yield_for(). Returns true on timeout.
template <typename F>
[[nodiscard]] inline auto yield_while_for(F&& condition, const platform::chrono::high_resolution_clock::duration& duration)
{
    return yield_while_until(condition, platform::chrono::high_resolution_clock::now() + duration);
};

inline auto wait(os::signal_type signal) -> std::optional<os::signal_type>
{
    os::signal_type received{};

    if ( auto rc = xTaskNotifyWait(0, signal, std::addressof(received), portMAX_DELAY); rc == pdTRUE )
        return received;

    return {};
}

template <class Rep, class Period>
inline auto wait(os::signal_type signal, const std::chrono::duration<Rep, Period>& timeout) -> std::optional<os::signal_type>
{
    os::signal_type received{};

    if ( auto rc = xTaskNotifyWait(0, signal, std::addressof(received), detail::ticks(timeout)); rc == pdTRUE )
    {
        return received;
    }

    return {};
}

} // namespace sys::this_thread
