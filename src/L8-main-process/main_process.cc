
// Includes.

#include <base.hpp>
#include <globals.hpp>
#include <hal.hpp>
#include <devices.hpp>
#include <filesystems.hpp>

#include <esp8266/esp8266_server_task.hpp>
#include <sensor_task.h>
#include <filesystem_task.h>
#include <display_task.h>
#include <wifi_task.h>
#include <usb_task.h>
#include <shell_task.h>

namespace sys {

namespace {

static constinit auto s_display_task    = os::static_task<"Display">{};
static constinit auto s_filesystem_task = os::static_task<"filesystem">{};
static constinit auto s_shell_task      = os::static_task<"Shell">{};
static constinit auto s_wifi_task       = os::static_task<"WiFi">{};
static constinit auto s_usb_task        = os::static_task<"USB">{};
static constinit auto s_sensor_task     = os::static_task<"Sensor">{};

} // namespace

void application::start()
{
    // Start scheduler.

    using namespace sys::dev;

    device_type<"/dev/led@1">::init();
    device_type<"/dev/i2c@3">::init();
    device_type<"/dev/24-dc-input@8">::init();
    device_type<"/dev/24-dc-input@7">::init();
    device_type<"/dev/24-dc-input@3">::init();

    auto os = log::info();

    os::scheduler::start(s_display_task);
    os::scheduler::start(s_filesystem_task);
    os::scheduler::start(s_shell_task);
    os::scheduler::start(s_wifi_task);
    os::scheduler::start(s_usb_task);
    os::scheduler::start(s_sensor_task);

    os::scheduler::run();
}

} // namespace sys
