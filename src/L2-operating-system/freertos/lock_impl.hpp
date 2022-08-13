#pragma once

namespace sys::os {

enum class lock_type
{
    k_mutex,
    k_semaphore
};

template <os::lock_type k_type, std::ptrdiff_t k_least_max_value = std::numeric_limits<std::ptrdiff_t>::max()>
requires(k_least_max_value < std::numeric_limits<UBaseType_t>::max())
struct static_lock final
{
    private:

    static auto tick_value(const std::chrono::milliseconds timeout) -> os::tick_type
    {
        if ( timeout == std::chrono::milliseconds::max() )
        {
            return portMAX_DELAY;
        }

        return timeout.count() / portTICK_PERIOD_MS;
    }

    auto do_lock(std::chrono::milliseconds timeout = std::chrono::milliseconds::max()) -> bool
    {
        BaseType_t rc;
        BaseType_t task_woken;

        if ( m_initialized == false )
        {
            return false;
        }

        if ( arch::is_in_interrupt_handler() )
        {
            rc = xSemaphoreTakeFromISR(m_handle, &task_woken);
            portYIELD_FROM_ISR(task_woken);
        }

        else
        {
            rc = xSemaphoreTake(m_handle, tick_value(timeout));
        }

        return rc == pdPASS;
    }

    auto do_unlock() -> bool
    {
        BaseType_t rc;
        BaseType_t task_woken;

        if ( m_initialized == false )
        {
            return false;
        }

        if ( arch::is_in_interrupt_handler() )
        {
            rc = xSemaphoreGiveFromISR(m_handle, &task_woken);
            portYIELD_FROM_ISR(task_woken);
        }

        else
        {
            rc = xSemaphoreGive(m_handle);
        }

        return rc == pdPASS;
    }

    static_assert(configUSE_MUTEXES != 0, "FreeRTOS Mutex support not enabled in FreeRTOSConfig.h file.");
    static_assert(configSUPPORT_STATIC_ALLOCATION != 0);

    public:

    constexpr static_lock() requires(k_type == os::lock_type::k_mutex)
    {
        m_handle = xSemaphoreCreateMutexStatic(std::addressof(m_buffer));
        if ( m_handle != nullptr )
        {
            m_initialized = true;
        }
    }

    constexpr explicit static_lock(std::ptrdiff_t initial_count = 0) requires(k_type == os::lock_type::k_semaphore && k_least_max_value == 1)
    {
        m_handle = xSemaphoreCreateBinaryStatic(std::addressof(m_buffer));
        if ( m_handle != nullptr )
        {
            m_initialized = true;
        }

        if ( initial_count > 0 )
        {
            release(1);
        }
    }

    constexpr explicit static_lock(std::ptrdiff_t initial_count = 0) requires(k_type == os::lock_type::k_semaphore && k_least_max_value > 1)
    {
        m_handle = xSemaphoreCreateCountingStatic(k_least_max_value, initial_count, std::addressof(m_buffer));
        if ( m_handle != nullptr )
        {
            m_initialized = true;
        }
    }

    static_lock(const static_lock&) = delete;
    static_lock(static_lock&&)      = delete;

    auto operator=(const static_lock&) -> static_lock& = delete;
    auto operator=(static_lock&&)      -> static_lock& = delete;

    void lock() requires(k_type == os::lock_type::k_mutex)
    {
        do_lock(std::chrono::milliseconds::max());
    }

    void acquire() requires(k_type == os::lock_type::k_semaphore)
    {
        do_lock(std::chrono::milliseconds::max());
    }

    void unlock() requires(k_type == os::lock_type::k_mutex)
    {
        do_unlock();
    }

    void release(std::ptrdiff_t update = 1) requires(k_type == os::lock_type::k_semaphore)
    {
        while ( update != 0 )
        {
            --update;
            do_unlock();
        }
    }

    bool try_lock() requires(k_type == os::lock_type::k_mutex)
    {
        return do_lock(std::chrono::milliseconds(0));
    }

    bool try_lock_acquire() requires(k_type == os::lock_type::k_mutex)
    {
        return do_lock(std::chrono::milliseconds(0));
    }

    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period>& relative_time) requires (k_type == os::lock_type::k_mutex)
    {
        return do_lock(std::chrono::duration<std::uint32_t, std::milli>(relative_time));
    }

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period>& relative_time) requires (k_type == os::lock_type::k_semaphore)
    {
        return do_lock(std::chrono::duration<std::uint32_t, std::milli>(relative_time));
    }

    template <class Duration>
    using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock, Duration>;

    template <class Clock, class Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time) requires(std::is_same_v<Clock, std::chrono::high_resolution_clock>)
    {
        using milli_dur = std::chrono::duration<std::uint32_t, std::milli>;

        auto rel_time = std::chrono::duration_cast<milli_dur>(abs_time - std::chrono::high_resolution_clock::now());

        return try_acquire_for(rel_time);
    }

    constexpr std::ptrdiff_t max() noexcept requires (k_type == os::lock_type::k_semaphore)
    {
        return k_least_max_value;
    }

    private:

    StaticSemaphore_t m_buffer;
    SemaphoreHandle_t m_handle;
    bool              m_initialized{};
};

} // namepace sys::os
