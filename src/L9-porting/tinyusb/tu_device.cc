
// Includes.

#include <base.hpp>
#include <hal.hpp>
#include <devices.hpp>

#include "tusb.h"

namespace sys {

//--------------------------------------------------------------------+
// device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted.

extern "C" void tud_mount_cb(void)
{
}

// Invoked when device is unmounted.

extern "C" void tud_umount_cb(void)
{
}

extern "C" void tud_cdc_rx_cb(uint8_t itf)
{
    static_cast<void>(itf);
    sys::dev::device_type<"/dev/serial@0">::on_data_available();
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus

extern "C" void tud_suspend_cb(bool remote_wakeup_en)
{
    static_cast<void>(remote_wakeup_en);
}

// Invoked when usb bus is resumed.

extern "C" void tud_resume_cb(void)
{
}

// Invoked when cdc when line state changed e.g connected/disconnected

extern "C" void tud_cdc_line_state_cb(std::uint8_t itf, bool dtr, bool rts)
{
    static_cast<void>(itf);

    if ( dtr && rts )
    {
        tud_cdc_write_str("\r\nTinyUSB CDC MSC device example\r\n");
    }
}

} // namespace sys
