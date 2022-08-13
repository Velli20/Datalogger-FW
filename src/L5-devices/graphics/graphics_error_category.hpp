#pragma once

namespace sys::dev::graphics {

enum class errc : std::uint32_t
{
    k_no_error,
    k_out_of_bounds,
    k_invalid_argument,
    k_driver_error
};

} // namespace sys::dev::graphics

template <>
struct sys::is_error_code_enum<sys::dev::graphics::errc> : std::true_type {};

namespace sys::dev::graphics {

static constexpr struct graphics_error_category : public sys::error_category
{
    constexpr graphics_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"ESP-AT"}; }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<graphics::errc>(value) )
            {
                case graphics::errc::k_no_error:
                    return "No error"sv;

                case graphics::errc::k_out_of_bounds:
                    return "Out of bounds"sv;

                case graphics::errc::k_invalid_argument:
                    return "Invalid argument"sv;

                case graphics::errc::k_driver_error:
                    return "Driver error"sv;
            }
        }

        return {};
    }
} k_graphics_error_category{};

[[nodiscard]] constexpr auto make_error_code(graphics::errc error)
{
    return sys::error_code{error, k_graphics_error_category};
}

} // namespace sys::dev::graphics
