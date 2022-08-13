#pragma once

namespace sys::arch::concepts {

template <device_node k_node, auto k_type>
concept has_interrupt_handler = requires()
{
    hal::peripheral_type<k_node>:: template interrupt_handler<k_type>();
};

template <device_node k_type>
concept has_irqn = sys::arch::detail::interrupt_count<k_type>::value > 0;

template <typename Func>
concept systick_handler = std::invocable<Func>;

template <typename Func>
concept svc_handler = requires(Func f, void* sp, os::service_call_code_type type, os::mode_type mode)
{
    { std::invoke(f, sp, type, mode) } -> std::same_as<void>;
};

template <typename Func>
concept pendable_svc_handler = std::invocable<Func>;

} // namespace sys::arch::concepts
