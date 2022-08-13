
// Includes.

#include <base.hpp>

namespace sys::hal {

void handle_bad_interrupt()
{
    log::error() << "An unexpected IRQ trap at vector xx" << /* system_control_block::active_interrupt_vector() << */ log::endl;
}

} // namespace sys::hal
