#pragma once

namespace sys::hal {

template <typename T>
concept has_init = requires(T type)
{
    type.Init;
};

template <class T, device_node k_node>
struct hal_handle final : public T
{
    using value_type = T;
    using pointer    = T*;
    using reference  = T&;

    using T::Instance;

    constexpr hal_handle()
    {
        constexpr auto k_base_address = register_address_of_node<k_node>::value;

        this->Instance = std::bit_cast<decltype(T::Instance)>(k_base_address);
    }

    operator pointer() noexcept
    {
        return this;
    }

    operator pointer() const noexcept
    {
        return this;
    }

    auto* get() const noexcept
    {
        return this;
    }

    auto error() requires(std::is_same<T, SD_HandleTypeDef>::value)
    {
        return static_cast<hal_sdmmc_error_type>(get()->ErrorCode);
    }

    auto error() requires(std::is_same<T, OSPI_HandleTypeDef>::value)
    {
        return static_cast<hal_ospi_error>(get()->ErrorCode);
    }

    auto error() requires(std::is_same<T, I2C_HandleTypeDef>::value)
    {
        return static_cast<hal_i2c_error_type>(get()->ErrorCode);
    }
};

} // namespace sys::hal
