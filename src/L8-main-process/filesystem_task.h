#pragma once

namespace sys {

template <>
struct os::static_task<"filesystem"> final : public os::task_base<8096, os::priority_type::k_priority_low>
{
    static constexpr os::signal_type k_signal_format_filesystem = (1 << 1);

    inline static void run(void*)
    {
        g_current_path.assign("/");

        if constexpr ( sys::config::chan_fatfs::value == true )
        {
            if ( g_fat_filesystem.mount() != true )
            {
                log::error() << "Unable to mount Fat filesystem " << g_fat_filesystem.error_code() << log::endl;
            }
        }

        if constexpr ( sys::config::littlefs::value == true )
        {
            if ( g_littlefs_filesystem.mount() != true )
            {
                log::debug() << "Unable to mount LittleFS filesystem " << g_littlefs_filesystem.error_code() << log::endl;
            }
        }

        using namespace sys::hal;

        while ( true )
        {
            // Wait for signals.

            const auto signal = this_thread::wait(k_signal_format_filesystem);
            if ( signal.has_value() != true )
            {
                continue;
            }
        }
    }
};

} // namespace sys
