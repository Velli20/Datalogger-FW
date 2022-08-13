#pragma once

namespace platform::memory {

auto default_resource() noexcept -> platform::pmr::memory_resource*;
auto heap_area()        noexcept -> std::tuple<std::uintptr_t, std::uintptr_t>;

auto initialize_free_store(
    std::byte*  heap_address_start,
    std::byte*  heap_address_end
) -> bool;

} // namespace platform::memory
