#pragma once

namespace sys {

struct socket
{
    constexpr socket() noexcept {}

    constexpr socket(const socket&) = delete;
    constexpr socket(socket&&)      = delete;

    [[maybe_unused]] constexpr auto operator=(socket&&)      noexcept -> socket& = delete;
    [[maybe_unused]] constexpr auto operator=(const socket&) noexcept -> socket& = delete;

    /// Destructor of this base class calls the destructors of derived classes.

    virtual ~socket() = default;

    virtual std::size_t write(std::span<const std::byte> data, std::chrono::milliseconds timeout) noexcept = 0;
    virtual bool        is_open() const                                                           noexcept = 0;
};

} // namespace sys
