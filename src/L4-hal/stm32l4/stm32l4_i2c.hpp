
#pragma once

namespace sys::hal {

template <device_node k_node> requires (detail::instance_of_i2c_node<k_node>::value)
struct peripheral_type<k_node> final
{
    private:

    static constexpr auto k_i2c_config = std::get<i2c_phy_config_type>(hal::peripheral_of(k_node).config);
    static constexpr auto k_scl_pin    = k_i2c_config.scl_pin;
    static constexpr auto k_sda_pin    = k_i2c_config.sda_pin;

    public:

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_i2c1_error || k_type == arch::arm::vendor_irqn_type::k_i2c2_error)
    static constexpr void interrupt_handler() noexcept
    {
        HAL_I2C_ER_IRQHandler(m_handle);
    }

    template <arch::arm::vendor_irqn_type k_type> requires (k_type == arch::arm::vendor_irqn_type::k_i2c1_event || k_type == arch::arm::vendor_irqn_type::k_i2c2_event)
    static constexpr void interrupt_handler() noexcept
    {
        HAL_I2C_EV_IRQHandler(m_handle);
    }

    static auto init(const sys::dev::config::i2c_config_type& config) noexcept -> bool
    {
        using namespace sys::dev;

        if ( auto was_initialized = m_initialized.exchange(true); was_initialized == true )
        {
            return true;
        }

        if ( config.mode != i2c_mode_type::k_master )
        {
            return m_initialized.exchange(false) == false;
        }

        arch::scoped_interrupt_lock guard{};

        // Enable and reset peripheral clock.

        hal::peripheral_type<"/chip/rcc">::enable<k_node>();
        hal::peripheral_type<"/chip/rcc">::reset <k_node>();

        // Configure SDA and SCL pins.

        hal::peripheral_type<k_sda_pin>::init();
        hal::peripheral_type<k_scl_pin>::init();

        m_handle.Init.Timing          = 0x307075B1;
        m_handle.Init.OwnAddress1     = 0;
        m_handle.Init.AddressingMode  = config.addressing == i2c_addressing_type::k_seven_bit ? I2C_ADDRESSINGMODE_7BIT : I2C_ADDRESSINGMODE_10BIT;
        m_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        m_handle.Init.OwnAddress2     = 0;
        m_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        m_handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

        if ( HAL_I2C_Init(m_handle) != HAL_OK )
        {
            //m_error = hal::make_error_code(m_handle.error());
            return m_initialized.exchange(false) == false;
        }

        // Enable the Analog I2C Filter.

        HAL_I2CEx_ConfigAnalogFilter(m_handle, I2C_ANALOGFILTER_ENABLE);

        return true;
    };

    static auto read(
        std::uint16_t             device_address,
        std::uint16_t             memory_address,
        std::span<std::byte>      buffer,
        std::chrono::milliseconds timeout
    ) noexcept -> std::size_t
    {
        if ( buffer.empty() )
        {
            return {};
        }

        if ( m_handle.Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT )
        {
            device_address = device_address << 1;
        }

        const auto rc = HAL_I2C_Mem_Read(m_handle, device_address, memory_address, 1, std::bit_cast<std::uint8_t*>(buffer.data()), buffer.size(), timeout.count());
        if ( rc != HAL_OK )
        {
            //m_error = hal::make_error_code(m_handle.error());
            return {};
        }

        return buffer.size();
    }

    static auto write(
        std::uint16_t              device_address,
        std::uint16_t              memory_address,
        std::span<const std::byte> buffer,
        std::chrono::milliseconds  timeout
    ) -> std::size_t
    {
        if ( buffer.empty() )
        {
            return {};
        }

        if ( m_handle.Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT )
        {
            device_address = device_address << 1;
        }

        const auto rc = HAL_I2C_Mem_Write(m_handle, device_address, memory_address, 1, std::bit_cast<std::uint8_t*>(buffer.data()), buffer.size(), timeout.count());
        if ( rc != HAL_OK )
        {
            //m_error = hal::make_error_code(m_handle.error());
            return {};
        }

        return buffer.size();
    }

    [[nodiscard]] static auto is_ready() noexcept -> bool
    {
        return m_initialized.load();
    }

    private:

    static inline           auto m_handle      = hal_handle<I2C_HandleTypeDef, k_node>{};
    static inline constinit auto m_initialized = std::atomic<bool>{};
};

} // namespace sys::hal
