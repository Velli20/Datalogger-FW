
// Includes.

#include <base.hpp>
#include <hal.hpp>

namespace {

using namespace sys;

[[gnu::section(".freertos_heap"), gnu::aligned(8)]]
std::array<std::byte, 30 * 1024> g_freertos_heap{};
auto s_kernel_allocator = sys::heap_allocator{g_freertos_heap.begin(), g_freertos_heap.end()};

[[gnu::section(".freertos_kernel"), gnu::aligned(8)]]
os::task_base<configMINIMAL_STACK_SIZE, os::priority_type::k_priority_lowest> s_idle_task;

[[gnu::section(".freertos_kernel"), gnu::aligned(8)]]
os::task_base<configMINIMAL_STACK_SIZE * 2, os::priority_type::k_priority_lowest> s_timer_task;

} // namespace

// pvPortMalloc

extern "C" void* pvPortMalloc(std::size_t xSize)
{
    void* ptr = s_kernel_allocator.allocate(xSize);
    debug_assert(ptr != nullptr);

    return ptr;
}

// vPortFree

extern "C" void vPortFree(void* pv)
{
    debug_assert(pv != nullptr);
    s_kernel_allocator.deallocate(pv);
}

// vApplicationGetIdleTaskMemory

extern "C" void vApplicationGetIdleTaskMemory(
    StaticTask_t** task_tcb_buffer,
    StackType_t ** task_stack_buffer,
    std::uint32_t* task_stack_size
)
{
    static_assert(configSUPPORT_STATIC_ALLOCATION == 1);

    *task_tcb_buffer   = std::addressof(s_idle_task.m_task_buffer);
    *task_stack_buffer = s_idle_task.m_task_stack;
    *task_stack_size   = s_idle_task.size / sizeof(StackType_t);
}

// vApplicationGetTimerTaskMemory

extern "C" void vApplicationGetTimerTaskMemory(
    StaticTask_t** task_tcb_buffer,
    StackType_t**  task_stack_buffer,
    std::uint32_t* task_stack_size
)
{
    *task_tcb_buffer   = std::addressof(s_timer_task.m_task_buffer);
    *task_stack_buffer = s_timer_task.m_task_stack;
    *task_stack_size   = s_timer_task.size / sizeof(StackType_t);
}

// extern "C" const volatile int uxTopUsedPriority = configMAX_PRIORITIES - 1;

extern "C" void vApplicationMallocFailedHook(void)
{
    using namespace sys;

    log::error() << "vApplicationMallocFailedHook: Malloc Failed in task: " << pcTaskGetName(nullptr) << log::endl;

    taskDISABLE_INTERRUPTS();
}

extern "C" void vApplicationStackOverflowHook(
    [[maybe_unused]] TaskHandle_t pxTask,
    [[maybe_unused]] char*        pcTaskName
)
{
    using namespace sys;

    log::error() << "vApplicationStackOverflowHook: Stack Overflow in task: " << pcTaskGetName(nullptr) << log::endl;

    taskDISABLE_INTERRUPTS();
}

extern "C" void vPortSetupTimerInterrupt(void)
{
    using namespace sys::hal;

    hal::peripheral_type<"/chip/cortex-m-systick">::on_tick(+[]() { xPortSysTickHandler(); });
}

extern "C" void configASSERT_callback(
    [[maybe_unused]] unsigned    line,
    [[maybe_unused]] const char* message
)
{
    using namespace sys;

    log::error() << "configASSERT_callback: at line '"  << line <<
                                            "' task '"  << pcTaskGetName(nullptr) <<
                                            "' error: " << message << log::endl;
    taskDISABLE_INTERRUPTS();
}

extern "C" std::uint32_t ulPortGetRunTime()
{
    return platform::chrono::high_resolution_clock::now().time_since_epoch().count();
}
