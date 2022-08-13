#pragma once

namespace sys::dev {

enum class i2c_mode_type
{
    k_master,
    kSlave
};

enum class i2c_addressing_type
{
    k_seven_bit,
    k_ten_bit
};

enum class i2c_speed_type
{
    k_standard,  /* 100 kHz */
    k_fast,      /* 400 kHz */
    k_fast_plus, /* 1 MHz   */
    k_high,      /* 3.4 MHz */
    k_ultra      /* 5 MHz   */
};

enum class i2c_op_type
{
    k_i2c_write, /* Write message */
    k_i2c_read,
    k_i2c_stop /* Send STOP */
};

struct i2c_transaction_type
{
    private:

    using message_type = std::variant<
        std::monostate,
        std::span<const std::byte>,
        std::span<std::byte>
    >;

    public:

    i2c_op_type   op;
    message_type  message{};
    std::uint16_t device_address;
    std::uint16_t memory_address;
};

} // namespace sys::dev
