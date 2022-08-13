#pragma once

namespace sys::dev {

template <device_node k_type>
struct device_type<k_type, typename std::enable_if<is_console_device<k_type>::value>::type>
{
    private:

    static constexpr auto k_device_configuration = std::get<serial_console_config>(device_tree::get(k_type));
    static constexpr auto k_peripheral           = k_device_configuration.peripheral;

    constexpr device_type() = delete;

    public:

    [[maybe_unused]] static auto init() noexcept
    {
        return hal::peripheral_type<k_peripheral>::init(k_device_configuration) ;
    }

    [[maybe_unused]] static auto write(std::span<const std::byte> buffer) -> std::size_t
    {
        return hal::peripheral_type<k_peripheral>::write(buffer);
    }

    [[maybe_unused]] static auto read(
        std::span<std::byte>      buffer,
        std::chrono::milliseconds timeout = std::chrono::milliseconds::max()
    ) noexcept -> std::size_t
    {
        return hal::peripheral_type<k_peripheral>::read(buffer, timeout);
    }
};

} // namespace sys::dev
