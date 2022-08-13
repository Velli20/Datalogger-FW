#pragma once

namespace sys {

class amutex
{
    std::atomic_flag m_locked{false};

    public:

    constexpr amutex() noexcept = default;

    amutex(amutex&&)            = delete;
    amutex(const amutex&)       = delete;

    inline auto lock() noexcept -> void
    {
        if ( arch::is_in_interrupt_handler() )
        {
            if ( try_lock() )
            {
                return;
            }

            // Deadlock.

            asm("bkpt");
        }

        this_thread::yield_while([this]() { return not try_lock(); });
    }

    void unlock() noexcept
    {
        m_locked.clear();
    }

    auto try_lock() noexcept -> bool
    {
        return not m_locked.test_and_set();
    }

    template <class Rep, class Period>
    auto try_lock_for(const std::chrono::duration<Rep, Period>& rel_time) noexcept -> bool
    {
        return not this_thread::yield_while_for([this] { return not this->try_lock(); }, rel_time);
    }

    template <class Clock, class Duration>
    auto try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time) noexcept -> bool
    {
        return not this_thread::yield_while_until([this] { return not this->try_lock(); }, abs_time);
    }
};

class asemaphore
{
    std::atomic<std::ptrdiff_t> m_counter{};

    public:

    constexpr asemaphore(std::ptrdiff_t initial_count = 0)
    {
        m_counter = initial_count;
    }

    asemaphore(asemaphore&&)      = delete;
    asemaphore(const asemaphore&) = delete;

    [[nodiscard]] auto try_acquire() noexcept -> bool
    {
        // Optimistically grab the resource.

        if ( auto previous = m_counter.fetch_sub(1/* , std::memory_order::acq_rel */); previous <= 0 )
        {
            // If it wasn't available undo the acquisition.

            m_counter.fetch_add(1/* , std::memory_order::release */);
            return false;
        }

        return true;
    }

    template <class Rep, class Period>
    [[nodiscard]] auto try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time) noexcept -> bool
    {
        return not this_thread::yield_while_for([this] { return not this->try_acquire(); }, rel_time);
    }

    auto acquire() noexcept -> void
    {
        this_thread::yield_while([this]() { return not try_acquire(); });
    }

    auto release() noexcept -> void
    {
        m_counter.fetch_add(1/* , std::memory_order::release */);
    }
};

} // namespace sys
