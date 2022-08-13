#pragma once

namespace sys::hal {

struct board final
{
    struct clock final
    {
        static bool init();
    };

    static bool init();
};

} // namespace sys::hal
