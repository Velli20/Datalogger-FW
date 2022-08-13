#pragma once

namespace sys::os::scheduler {

namespace {

using ticks_type = std::chrono::duration<
    TickType_t,
    std::ratio<portTICK_PERIOD_MS,
    configTICK_RATE_HZ>
>;

static_assert(pdMS_TO_TICKS(1000) == ticks_type(std::chrono::seconds(1)).count());
static_assert(pdMS_TO_TICKS(100)  == std::chrono::floor<ticks_type>(std::chrono::milliseconds(100)).count());
static_assert(pdMS_TO_TICKS(10)   == std::chrono::floor<ticks_type>(std::chrono::milliseconds(10)).count());

void svc_handler(
    void*                      sp,
    os::service_call_code_type type,
    os::mode_type /* mode */
) noexcept
{
    os::stack_frame_type* stackframe = static_cast<os::stack_frame_type*>(sp);

    switch ( type )
    {
        case os::service_call_code_type::k_kernel_start:

            if constexpr ( sys::config::build_type::debug == true )
            {
                log::debug() << "[SVC] Kernel start" << log::endl;
            }

            vPortSVCHandler();
            break;

        case os::service_call_code_type::k_allocate_memory:

            if ( auto* ptr = pvPortMalloc(stackframe->r0.sz); ptr != nullptr )
            {
                stackframe->r0.ptr = ptr;
                stackframe->r1.b   = true;
            }

            else
            {
                stackframe->r0.ptr = nullptr;
                stackframe->r1.b   = false;
            }

            if constexpr ( sys::config::build_type::debug == true )
            {
                log::info() << "[SVC] Malloc " << ( stackframe->r0.ptr != nullptr ? "OK" : "FAIL") << log::endl;
            }

            break;

        case os::service_call_code_type::k_free_memory:

            vPortFree(stackframe->r0.ptr);

            stackframe->r1.b = true;
            break;
    }
}

} // namespace

inline void run()
{
    hal::peripheral_type<"/chip/cpu@0">::on_service_call(svc_handler);
    hal::peripheral_type<"/chip/cpu@0">::on_pendable_service_call(xPortPendSVHandler);

    vTaskStartScheduler();
}

template <typename T> /* requires(std::is_base_of_v<os::task_base, T>) */
constexpr void start(T& task)
{
    task.m_task_handle =
        xTaskCreateStatic(
            &task.run,
            "Hello",
            T::size / sizeof(StackType_t),
            nullptr,
            configMAX_PRIORITIES - static_cast<UBaseType_t>(T::priority_type),
            task.m_task_stack,
            std::addressof(task.m_task_buffer));
}

template <string::static_string k_task_name>
bool signal(const os::signal_type signal)
{
    constexpr auto k_task_id = std::get<0>(std::forward_as_tuple(k_task_name));

    BaseType_t rc;
    BaseType_t task_woken;

    if ( arch::is_in_interrupt_handler() )
    {
        rc = xTaskNotifyFromISR(os::static_task<k_task_id>::m_task_handle, signal, eSetBits, &task_woken);
        portYIELD_FROM_ISR(task_woken);
    }

    else
    {
        rc = xTaskNotify(os::static_task<k_task_id>::m_task_handle, signal, eSetBits);
    }

    return rc == pdTRUE ? true : false;
}

} // namspace sys::os::scheduler

