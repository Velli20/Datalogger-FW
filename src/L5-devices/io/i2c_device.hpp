#pragma once

namespace sys::dev {

template <device_node k_type>
struct device_type<k_type, typename std::enable_if<is_i2c_device<k_type>::value>::type>
{
    private:

    constexpr device_type() = delete;

    static constexpr auto k_device_configuration = std::get<config::i2c_config_type>(device_tree::get(k_type));
    static constexpr auto k_peripheral           = k_device_configuration.peripheral;

    public:

    static bool init()
    {
        return hal::peripheral_type<k_peripheral>::init(k_device_configuration);
    }

    static auto read(
        std::uint16_t             device_address,
        std::uint16_t             memory_address,
        std::span<std::byte>      buffer,
        std::chrono::milliseconds timeout = std::chrono::milliseconds::max()
    )
    {
        return hal::peripheral_type<k_peripheral>::read(device_address, memory_address, buffer, timeout);
    }

    static auto write(
        std::uint16_t                    device_address,
        std::uint16_t                    memory_address,
        const std::span<const std::byte> buffer,
        std::chrono::milliseconds        timeout = std::chrono::milliseconds::max()
    )
    {
        return hal::peripheral_type<k_peripheral>::write(device_address, memory_address, buffer, timeout);
    }

    static auto transfer(const i2c_transaction_type& transaction)
    {
        if ( transaction.op == i2c_op_type::k_i2c_read )
        {
            return read(transaction.device_address, transaction.memory_address, std::get<std::span<std::byte>>(transaction.message()));
        }

        else if ( transaction.op == i2c_op_type::k_i2c_write )
        {
            return write(transaction.device_address, transaction.memory_address, std::get<std::span<const std::byte>>(transaction.message()));
        }

        return 0u;
    }
};

} // namespace sys::dev
