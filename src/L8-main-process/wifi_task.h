#pragma once

namespace sys {

template <>
struct os::static_task<"WiFi"> final : public os::task_base<4048, os::priority_type::k_priority_low>
{
    private:

    static constexpr auto k_wifi_instance = device_node{"/dev/wifi@0"};

    public:

    inline static void run(void*)
    {
        using namespace sys::hal;
        using namespace sys::dev;
        using namespace sys::dev::esp8266;
        using namespace std::string_view_literals;
        using namespace std::chrono_literals;

        device_type<k_wifi_instance>::init();

        // os::scheduler::start(g_wifi_task);

        while ( true )
        {
            // Wait for signals.
            device_type<k_wifi_instance>::run();

            this_thread::sleep_for(300ms);
        }
    }
};

} // namespace sys
