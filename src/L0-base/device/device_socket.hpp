#pragma once

namespace sys::dev {

template <typename T>
struct socket
{
    constexpr socket() = default;

    auto open(T id)
    {
        m_id = id;
    }

    virtual auto close() -> bool = delete;

    virtual auto write(std::span<const std::byte> data, std::chrono::milliseconds timeout) -> std::size_t = delete;

    [[nodiscard]] auto error() -> std::errc
    {
        return {};
    }

    private:

    T m_id{};
};

} // namespace sys::dev
