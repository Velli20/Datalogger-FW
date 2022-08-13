#pragma once

namespace sys::dev {

enum class input_event : std::uint32_t
{
    k_none = 0U,
    k_key_down,
    k_key_up,
};

using input_callback_type = std::function<void(input_event, key_code_type)>;

} // namespace sys::dev

