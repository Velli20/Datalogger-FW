#pragma once

namespace sys::dev {

template <device_node k_type>
struct device_type<k_type, typename std::enable_if<is_flash_memory_device<k_type>::value>::type>
{
    private:

    static constexpr auto k_flash_config = std::get<memory_config_type<memory_type::k_nor_flash>>(device_tree::get(k_type));
    static constexpr auto k_peripheral   = k_flash_config.peripheral;

    public:

    static auto init()
    {
        if ( hal::peripheral_type<k_peripheral>::init() != true )
        {
            log::error() << hal::peripheral_type<k_peripheral>::GetError() << log::endl;
            return false;
        }

        return true;
    }

    static auto erase_sector(std::uint32_t sector_address) -> bool
    {
        return hal::peripheral_type<k_peripheral>::erase_sector(sector_address);
    }

    static auto erase_chip() -> bool
    {
        if ( hal::peripheral_type<k_peripheral>::erase_chip() == false )
        {
            log::error() << hal::peripheral_type<k_peripheral>::GetError() << log::endl;
            return false;
        }

        return true;
    }

    [[maybe_unused]] static auto read(
        std::span<std::byte> buffer,
        std::uint32_t        sector_address
    ) noexcept -> std::size_t
    {
        auto result = hal::peripheral_type<k_peripheral>::read(buffer, sector_address);
        if ( result != buffer.size_bytes() )
        {
            log::error() << hal::peripheral_type<k_peripheral>::GetError() << log::endl;
        }

        return result;
    }

    [[maybe_unused]] static auto write(
        std::span<const std::byte> buffer,
        std::uint32_t              sector_address
    ) noexcept -> std::size_t
    {
        auto result = hal::peripheral_type<k_peripheral>::write(buffer, sector_address);
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
};

} // namespace sys::dev

