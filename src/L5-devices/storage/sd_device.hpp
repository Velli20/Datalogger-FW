#pragma once

namespace sys::dev {

template <device_node k_descriptor>
struct device_type<k_descriptor, typename std::enable_if<is_sd_card_device<k_descriptor>::value>::type>
{
    private:

    static constexpr auto k_device_configuration = std::get<memory_config_type<memory_type::k_sd>>(device_tree::get(k_descriptor));
    static constexpr auto k_peripheral           = k_device_configuration.peripheral;

    public:

    static bool init()
    {
        if ( hal::peripheral_type<k_peripheral>::init() != true )
        {
            log::error() << hal::peripheral_type<k_peripheral>::GetError() << log::endl;
            return false;
        }

        return true;
    }

    template <typename T>
    [[maybe_unused]] static auto read_blocks(
        const std::span<T> buffer,
        std::uint32_t      block_address
    ) noexcept -> std::size_t
    {
        auto result = hal::peripheral_type<k_peripheral>::read_blocks(buffer, block_address);
        if ( result != buffer.size() )
        {
            log::error() << hal::peripheral_type<k_peripheral>::GetError() << log::endl;
        }

        return result;
    }

    template <typename T>
    [[maybe_unused]] static auto write_blocks(
        const std::span<const T> buffer,
        std::uint32_t            block_address
    ) noexcept -> std::size_t
    {
        auto result = hal::peripheral_type<k_peripheral>::write_blocks(buffer, block_address);
        if ( result != buffer.size() )
        {
            log::error() << hal::peripheral_type<k_peripheral>::GetError() << log::endl;
        }

        return result;
    }

    [[nodiscard]] static auto block_count()
    {
        return hal::peripheral_type<k_peripheral>::block_count();
    }

    [[nodiscard]] static auto block_size()
    {
        return hal::peripheral_type<k_peripheral>::block_size();
    }

    [[nodiscard]] static auto is_ready(std::uint8_t /* LogicalUnitNumber */)
    {
        return hal::peripheral_type<k_peripheral>::is_ready();
    }
};

} // namespace sys::dev

