#pragma once

#include <command/command_rtc.hpp>
#include <command/command_kernel.hpp>
#include <command/command_led.hpp>
#include <command/command_i2c.hpp>
#include <command/command_arm.hpp>
#include <command/command_file_system.hpp>
#include <command/command_display.hpp>
#include <command/command_high_voltage_io.hpp>
#include <command/command_wifi.hpp>
#include <command/command_flash.hpp>

#include <shell/shell.hpp>

namespace sys {

template <>
struct os::static_task<"Shell"> final : public os::task_base<8048, os::priority_type::k_priority_low>
{
    static constexpr auto k_command_list = shell::command_list<
        "rtc-get-time",
        "rtc-set-time",
        "rtc-set-date",
        "led-on",
        "led-off",
        "led-toggle",
        "kernel-stats",
        "i2c-scan",
        "i2c-write",
        "i2c-read",
        "test-arm-io-illegal-instruction",
        "test-arm-div-zero-trap",
        "test-arm-float-access",
        "test-arm-bad-address",
        "ls",
        "cat",
        "cd",
        "mkdir",
        "pwd",
        "fatfs-format",
        "display-on",
        "display-off",
        "display-invert",
        "hv-channel-read",
        "wifi-scan",
        "wifi-time",
        "wifi-http-get",
        "wifi-ap-connect",
        "wifi-ap-disconnect",
        "wifi-ap-start",
        "wifi-ap-clients",
        "wifi-deep-sleep",
        "wifi-ip",
        "flash-read",
        "flash-write",
        "flash-erase",
        "flash-mmap",
        "littlefs-format",
        "littlefs-test"
    >();

    inline static void run(void*)
    {
        s_shell.run();

        vTaskDelete(nullptr);
    }

    static inline sys::command_shell s_shell{std::span{k_command_list}};
};

} // namespace sys
