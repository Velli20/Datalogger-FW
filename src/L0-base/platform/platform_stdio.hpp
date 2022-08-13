#pragma once

namespace platform::stdio {

auto write(std::span<const std::byte> buffer) noexcept -> std::size_t;
auto read(std::span<std::byte> buffer)        noexcept -> std::size_t;

} // namespace platform::stdio
