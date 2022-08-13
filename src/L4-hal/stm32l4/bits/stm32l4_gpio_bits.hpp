#pragma once

namespace sys::hal {

template <std::uint32_t k_pin>
struct GpioLayout
{
    /*!< GPIO port mode register,               Address offset: 0x00      */

    mmio::bitmask_type<mmio::field_size_type::u32, 2ul, k_pin * 2ul, gpio_mode_type>                        Mode;

    /*!< GPIO port output type register,        Address offset: 0x04      */

    mmio::bitmask_type<mmio::field_size_type::u32, 2ul, k_pin * 2ul>                                  OutputType;

    /*!< GPIO port output speed register,       Address offset: 0x08      */

    mmio::bitmask_type<mmio::field_size_type::u32, 2ul, k_pin * 2ul, gpio_speed_type>                       OutputSpeed;

    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */

    mmio::bitmask_type<mmio::field_size_type::u32, 2ul, k_pin * 2ul, gpio_pull_type>                        PortPull;

    /*!< GPIO port input data register,         Address offset: 0x10      */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, k_pin>                            InputData;

    /*!< GPIO port output data register,        Address offset: 0x14      */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, k_pin>                            OutputData;

    /*!< GPIO port bit set/reset  register,     Address offset: 0x18      */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, k_pin>                            BitSetReset;

    /*!< GPIO port configuration lock register, Address offset: 0x1C      */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, k_pin * 2ul>                      ConfigLock;

    private:

    /*!< GPIO alternate function low register,  Address offset: 0x20      */

    mmio::register_type<mmio::field_size_type::u32>                                           AlternateLow;

    /*!< GPIO alternate function high register,  Address offset: 0x24     */

    mmio::register_type<mmio::field_size_type::u32>                                           AlternateHigh;

    public:

    constexpr auto Alternate() noexcept requires(concepts::is_alternate_high_register<k_pin>)
    {
        return AlternateHigh.Field<4ul, (k_pin * 4ul) % 32ul, gpio_alternate_type>();
    }

    constexpr auto Alternate() noexcept requires(concepts::is_alternate_low_register<k_pin>)
    {
        return AlternateLow.Field<4ul, (k_pin * 4ul) % 32ul, gpio_alternate_type>();
    }

    /*!< GPIO Bit Reset register,               Address offset: 0x28      */

    mmio::bitmask_type<mmio::field_size_type::u32, 1ul, k_pin>                            BitReset;
};

static_assert(sizeof(GpioLayout<0>) == 44);

} // namespace sys::hal
