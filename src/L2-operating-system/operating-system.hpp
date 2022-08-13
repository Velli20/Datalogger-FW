#pragma once

extern "C" void vPortSVCHandler(void);
extern "C" void xPortSysTickHandler(void);
extern "C" void SVC_Handler(void);
extern "C" void PendSV_Handler(void);

#include <freertos/os_types.hpp>
#include <freertos/lock_impl.hpp>
#include <freertos/scheduler.hpp>
#include <freertos/this_thread.hpp>
#include <atomic_lock.hpp>

namespace platform {

using mutex            = sys::os::static_lock<sys::os::lock_type::k_mutex>;
using binary_semaphore = sys::os::static_lock<sys::os::lock_type::k_semaphore, 1>;

} // namespace platform
