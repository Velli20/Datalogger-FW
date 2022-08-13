#pragma once

namespace sys::dev::esp8266  {

constinit struct wifi_task : public os::task_base<2048, os::priority_type::k_priority_low>
{
    inline static /* constexpr */ void run(void*)
    {
        using namespace std::chrono_literals;

        dev::device_type<"/dev/wifi@0">::create_access_point(std::string_view{"test"}, dev::esp8266::encryption_type::k_open);
        dev::device_type<"/dev/wifi@0">::start(80, dev::esp8266::connection_type::k_tcp);

        while ( 1 )
        {
            this_thread::sleep_for(300ms);
            dev::device_type<"/dev/wifi@0">::try_serve();
        }
    }
} g_wifi_task{};

} // namespace sys::dev::esp8266
