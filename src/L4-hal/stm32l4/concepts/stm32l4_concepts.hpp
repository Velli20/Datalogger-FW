#pragma once

namespace sys::hal::concepts {

template <std::uint32_t k_pin>
concept is_alternate_high_register = k_pin > 7ul;

template <std::uint32_t k_pin>
concept is_alternate_low_register = k_pin <= 7ul;

template <typename F>
concept exti_handler = std::invocable<F>;

} // namespace sys::hal::concepts

