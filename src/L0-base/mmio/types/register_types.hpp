#pragma once

namespace sys::mmio {

enum class field_size_type : std::uint8_t
{
     u8,
    u16,
    u32,
    u64,
};

template <typename Mask>
using register_data_type = typename Mask::data_value_type;

} // namespace sys::mmio
