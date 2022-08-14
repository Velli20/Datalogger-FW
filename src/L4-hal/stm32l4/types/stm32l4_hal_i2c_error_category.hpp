#pragma once

namespace sys::hal {

enum class hal_i2c_error_type : std::int32_t
{
    k_no_error             = HAL_I2C_ERROR_NONE,
    k_bus_error            = HAL_I2C_ERROR_BERR,
    k_arbitration_loss     = HAL_I2C_ERROR_ARLO,
    k_no_acknowledge       = HAL_I2C_ERROR_AF,
    k_overrun_error        = HAL_I2C_ERROR_OVR,
    k_dma_error            = HAL_I2C_ERROR_DMA,
    k_timeout_error        = HAL_I2C_ERROR_TIMEOUT,
    k_size_managment_error = HAL_I2C_ERROR_SIZE,
    k_dma_parameter_error  = HAL_I2C_ERROR_DMA_PARAM,
#if defined(USE_HAL_I2C_REGISTER_CALLBACKS) && (USE_HAL_I2C_REGISTER_CALLBACKS == 1U)
    k_invalid_callback     = HAL_I2C_ERROR_INVALID_CALLBACK,
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    k_invalid_parameter    = HAL_I2C_ERROR_INVALID_PARAM,
};

} // namespace sys::hal

template <>
struct sys::is_error_code_enum<sys::hal::hal_i2c_error_type> : std::true_type {};

namespace sys::hal {

static constexpr struct i2c_error_category : public sys::error_category
{
    constexpr i2c_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"HAL I2C"}; }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<hal_i2c_error_type>(value) )
            {
                case hal_i2c_error_type::k_no_error:
                    return "No error"sv;

                case hal_i2c_error_type::k_bus_error:
                    return "Bus error"sv;

                case hal_i2c_error_type::k_arbitration_loss:
                    return "Arbitration Loss error"sv;

                case hal_i2c_error_type::k_no_acknowledge:
                    return "Non-Acknowledge error"sv;

                case hal_i2c_error_type::k_overrun_error:
                    return "Overrun error"sv;

                case hal_i2c_error_type::k_dma_error:
                    return "DMA transfer error"sv;

                case hal_i2c_error_type::k_timeout_error:
                    return "Timeout error"sv;

                case hal_i2c_error_type::k_size_managment_error:
                    return "svize Management error"sv;

                case hal_i2c_error_type::k_dma_parameter_error:
                    return "DMA Parameter Error"sv;

#if defined(USE_HAL_I2C_REGISTER_CALLBACKS) && (USE_HAL_I2C_REGISTER_CALLBACKS == 1U)
                case hal_i2c_error_type::k_invalid_callback:
                    return "Invalid Callback error"sv;
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

                case hal_i2c_error_type::k_invalid_parameter:
                    return "Invalid Parameters error"sv;
            }
        }

        return {};
    }
} k_i2c_error_category{};

[[nodiscard]] constexpr auto make_error_code(hal_i2c_error_type error)
{
    return sys::error_code{error, k_i2c_error_category};
}

} // namespace sys::hal
