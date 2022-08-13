#pragma once

namespace sys::dev::esp8266 {

template <device_node instance = "/dev/wifi@0">
struct lock final
{
    lock()
    {
        device_type<instance>::lock();
    }

    ~lock()
    {
        device_type<instance>::unlock();
    }
};

} // namespace sys::dev::esp8266
