#pragma once

namespace sys::hal {

enum class hal_ospi_error : std::int32_t
{
    no_error          = HAL_OSPI_ERROR_NONE,
    timeout_error     = HAL_OSPI_ERROR_TIMEOUT,
    transfer_error    = HAL_OSPI_ERROR_TRANSFER,
    dma_error         = HAL_OSPI_ERROR_DMA,
    invalid_parameter = HAL_OSPI_ERROR_INVALID_PARAM,
    invalid_sequence  = HAL_OSPI_ERROR_INVALID_SEQUENCE,
#if defined (USE_HAL_OSPI_REGISTER_CALLBACKS) && (USE_HAL_OSPI_REGISTER_CALLBACKS == 1U)
    invalid_callback  = HAL_OSPI_ERROR_INVALID_CALLBACK
#endif
};

} // namespace sys::hal

template <>
struct sys::is_error_code_enum<sys::hal::hal_ospi_error> : std::true_type {};

namespace sys::hal {

static constexpr struct ospi_error_category : public sys::error_category
{
    constexpr ospi_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"HAL OSPI"}; }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<hal_ospi_error>(value) )
            {
                case hal_ospi_error::no_error:
                    return "No error"sv;

                case hal_ospi_error::timeout_error:
                    return "Timeout error"sv;

                case hal_ospi_error::transfer_error:
                    return "Transfer error"sv;

                case hal_ospi_error::dma_error:
                    return "DMA transfer error"sv;

                case hal_ospi_error::invalid_parameter:
                    return "Invalid parameters error"sv;

                case hal_ospi_error::invalid_sequence:
                    return "Sequence of the state machine is incorrect"sv;

#if defined (USE_HAL_OSPI_REGISTER_CALLBACKS) && (USE_HAL_OSPI_REGISTER_CALLBACKS == 1U)
                case hal_ospi_error::invalid_callback:
                    return "Invalid callback error"sv;
#endif
            }
        }

        return {};
    }
} k_ospi_error_category{};

[[nodiscard]] constexpr auto make_error_code(hal_ospi_error error)
{
    return sys::error_code{error, k_ospi_error_category};
}

} // namespace sys::hal
