
#pragma once

template <>
struct sys::is_error_code_enum<HAL_StatusTypeDef> : std::true_type {};

namespace sys::hal {

static constexpr struct hal_status_error_category : public sys::error_category
{
    constexpr hal_status_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"HAL"};  }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        using namespace std::string_view_literals;

        if constexpr ( sys::config::build_type::debug == true )
        {
            switch ( static_cast<HAL_StatusTypeDef>(value) )
            {
                case HAL_StatusTypeDef::HAL_OK:
                    return "Succeeded."sv;

                case HAL_StatusTypeDef::HAL_ERROR:
                    return "Error."sv;

                case HAL_StatusTypeDef::HAL_BUSY:
                    return "Busy."sv;

                case HAL_StatusTypeDef::HAL_TIMEOUT:
                    return "Timeout."sv;
            }
        }

        return {};
    }
} k_hal_status_error_category{};

[[nodiscard]] constexpr auto make_error_code(HAL_StatusTypeDef error)
{
    return sys::error_code{error, k_hal_status_error_category};
}

} // namespace sys::hal
