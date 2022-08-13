#pragma once

namespace sys::mmio::concepts {

template <typename WriteValueType, typename Mask>
concept is_writable_data_value = std::is_same_v<WriteValueType, mmio::register_data_type<Mask>> && std::negation<std::is_enum<mmio::register_data_type<Mask>>>::value;

template <typename WriteValueType, typename Mask>
concept is_writable_enum_value = std::is_same_v<WriteValueType, mmio::register_data_type<Mask>> && std::is_enum<mmio::register_data_type<Mask>>::value;

} // namespace sys::mmio::concepts
