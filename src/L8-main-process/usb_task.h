#pragma once

// Includes.

#include <tusb.h>

namespace sys {

template <>
struct os::static_task<"USB"> final : public os::task_base<1024, os::priority_type::k_priority_low>
{
    public:

    inline static void run(void*)
    {
        tusb_init();

        while ( true )
        {
            tud_task();
        }
    }
};

} // namespace sys

