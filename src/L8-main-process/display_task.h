#pragma once

namespace sys {

template <>
struct os::static_task<"Display"> final : public os::task_base<1024, os::priority_type::k_priority_medium>
{
    static constexpr os::signal_type k_signal_display_test   = 1 << 1;
    static constexpr os::signal_type k_signal_display_on     = 1 << 2;
    static constexpr os::signal_type k_signal_display_off    = 1 << 3;
    static constexpr os::signal_type k_signal_display_invert = 1 << 4;

    private:

    static void display_redraw() noexcept
    {
        // Clear screen.

        m_graphics.buffer_clear();

        std::tm tim;

        using namespace sys::hal;

        // Get RTC timestamp.

        if ( hal::peripheral_type<"/chip/rtc">::get_time(tim) == true )
        {
            char buffer[64];

            std::snprintf(buffer, sizeof(buffer),
                "%02d:%02d:%02d",
                tim.tm_hour,
                tim.tm_min,
                tim.tm_sec
            );

            // Draw timestamp.

            m_graphics.draw_string(0, 0, std::addressof(font_roboto), buffer);
        }

        // Refresh display.

        m_graphics.buffer_refresh();
    }

    static void display_enable(bool enable) noexcept
    {
        if ( enable )
        {
            m_graphics.display_driver_set_display_on();
        }

        else
        {
            m_graphics.display_driver_set_display_off();
        }
    }

    static void display_invert() noexcept
    {
        m_graphics.display_driver_invert();
    }

    public:

    inline static /* constexpr */ void run(void*)
    {
        using namespace sys::hal;

        if ( sys::dev::device_type<"/dev/i2c@1">::init() == false )
        {
            log::debug() << "I2C device initialization failed" << log::endl;
            vTaskDelete(nullptr);
        }

        if ( auto rc = m_graphics.init(); rc.has_value() == false )
        {
            log::error() << "Failed to initialize display" << log::endl;
        }

        auto status = k_signal_display_on;

        while ( true )
        {
            using namespace ::std::chrono_literals;

            const auto timeout = (status & k_signal_display_on) ? 1s : std::chrono::milliseconds::max();

            const auto signal = this_thread::wait(
                k_signal_display_test |
                k_signal_display_off  |
                k_signal_display_on   |
                k_signal_display_invert,
                timeout
            );

            // Refresh display.

            if ( (status & k_signal_display_on) )
            {
                display_redraw();
            }

            if ( signal.has_value() == false )
            {
                continue;
            }

            // Display on.

            if ( (signal.value() & k_signal_display_on) )
            {
                status &= ~k_signal_display_off;
                status |=  k_signal_display_on;

                display_enable(true);
            }

            // Display off.

            if ( (signal.value() & k_signal_display_off) )
            {
                status &= ~k_signal_display_on;
                status |=  k_signal_display_off;

                display_enable(false);
            }

            // Invert colors.

            if ( (signal.value() & k_signal_display_invert) )
            {
                display_invert();
            }
        }
    }

    private:

    static inline dev::monochrome_graphics<128, 64, "/dev/i2c@1"> m_graphics{};
};

} // namespace sys
