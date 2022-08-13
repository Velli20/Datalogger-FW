#pragma once

// FreeRTOS includes.

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

namespace sys::os {

using tick_type          = std::uint32_t;
using signal_type        = std::uint32_t;
using thread_handle_type = TaskHandle_t;
using thread_id_type     = decltype(util::hash_fnv1a(""));

struct stack_frame_type
{
    using function_type = void(*)(void);

    union
    {
        std::size_t   sz;
        std::uint32_t u32;
        void*         ptr;
    } r0;

    union
    {
        std::uint32_t u32;
        void*         ptr;
        bool          b;
    } r1;

    std::uint32_t r2;
    std::uint32_t r3;
    std::uint32_t r12;
    std::uint32_t lr;
    function_type pc;
    std::uint32_t psr;
};

static_assert(sizeof(os::stack_frame_type) == 32);

// Task priority enumeration.

enum class priority_type : UBaseType_t
{
    k_priority_highest = 1,
    k_priority_high    = 2,
    k_priority_medium  = 3,
    k_priority_low     = 4,
    k_priority_lowest  = 5
};

template <string::static_string k_name>
struct static_task
{};

template <std::uint32_t k_task_size, os::priority_type k_priority>
struct task_base
{
    static constexpr auto size          = k_task_size;
    static constexpr auto priority_type = k_priority;

    static inline constinit auto m_task_buffer = StaticTask_t{};
    static inline constinit auto m_task_handle = os::thread_handle_type{};
    static inline constinit auto m_id          = os::thread_id_type{};

    // ARM Architecture Procedure Call Standard [AAPCS] requires 8-byte stack alignment.

    alignas(8) static inline StackType_t m_task_stack[k_task_size / sizeof(StackType_t)]{};
};

} // namspace sys::os
