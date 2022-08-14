#pragma once

namespace sys::hal {

enum class uart_error_type : std::int32_t
{
    k_no_error,
    k_initialization_error,
    k_timeout,
    k_invalid_parameter
};

} // namespace sys::hal

template <>
struct sys::is_error_code_enum<sys::hal::uart_error_type> : std::true_type {};

namespace sys::hal {

static constexpr struct uart_error_category : public sys::error_category
{
    constexpr uart_error_category()
        : error_category{}
        {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override
    {
        return std::string_view{"UART"};
    }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<uart_error_type>(value) )
            {
                case uart_error_type::k_no_error:
                    return "No error"sv;

                case uart_error_type::k_initialization_error:
                    return "Initialization error"sv;

                case uart_error_type::k_timeout:
                    return "Timeout"sv;

                case uart_error_type::k_invalid_parameter:
                    return "Invalid parameter"sv;
            }
        }

        return {};
    }

} k_uart_error_category{};

[[nodiscard]] constexpr auto make_error_code(uart_error_type error)
{
    return sys::error_code{error, k_uart_error_category};
}

} // namespace sys::hal
