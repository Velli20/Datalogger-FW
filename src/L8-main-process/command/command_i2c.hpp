#pragma once

namespace sys {

template <>
struct shell::command_type<"i2c-scan"> final
{
    static constexpr auto k_test_byte = std::array<std::byte, 1>{};

    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;
        using namespace std::string_view_literals;

        auto index = std::atoi(args[0].data());
        if ( index < 1 || index > 3 )
        {
            log::error() << "Invalid argument. Expected device index 1-3"sv << log::endl;
            return;
        }

        static constexpr auto k_header = "       0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F"sv;

        os << log::endl << k_header << log::endl;
        os << log::format::hex << log::format::showbase;

        for ( std::uint16_t i{}; i < 0x77; i += 16 )
        {
            os << static_cast<std::uint8_t>(i) << ": "sv;

            for ( std::uint16_t j{}; j != 16; ++j )
            {
                if ( device_type<"/dev/i2c@3">::write(i + j, 0, std::span{k_test_byte}, 100ms) == 0u )
                {
                    os << "---- "sv;
                    continue;
                }

                os << static_cast<std::uint8_t>(i + j) << ' ';
            }

            os << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Scan I2C devices <port 1-3>"sv;
    }
};

template <>
struct shell::command_type<"i2c-write"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;
        using namespace std::string_view_literals;

        auto index = std::atoi(args[0].data());
        if ( index < 1 || index > 3 )
        {
            os << "Invalid argument. Expected device index 1-3"sv << log::endl;
            return;
        }

        auto dev_address = static_cast<std::uint16_t>(std::strtol(args[1].data(), nullptr, 16));
        auto reg_address = static_cast<std::uint16_t>(std::strtol(args[2].data(), nullptr, 16));
        auto out_byte    = static_cast<std::byte>    (std::strtol(args[3].data(), nullptr, 16));

        log::info() << "Write I2C" << log::format::hex << log::format::showbase << log::endl
                    << " device:   " << static_cast<std::uint8_t>(dev_address)  << log::endl
                    << " register: " << static_cast<std::uint8_t>(reg_address)  << log::endl
                    << " byte:     " << static_cast<std::uint8_t>(out_byte)     << log::endl;

        device_type<"/dev/i2c@3">::write(dev_address, reg_address, std::span{std::addressof(out_byte), 1}, 100ms);
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Write to I2C device <port 1-3> <address> <register> <byte>."sv;
    }

    static consteval auto name() { return std::string_view{"i2c-write"}; }
};

template <>
struct shell::command_type<"i2c-read"> final
{
    static auto on_command_match(const shell::arguments& args)
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;
        using namespace std::string_view_literals;

        auto index = std::atoi(args[0].data());
        if ( index < 1 || index > 3 )
        {
            log::error() << "Invalid argument. Expected device index 1-3"sv << log::endl;
            return;
        }

        auto dev_address = static_cast<std::uint16_t>(std::strtol(args[1].data(), nullptr, 16));
        auto reg_address = static_cast<std::uint16_t>(std::strtol(args[2].data(), nullptr, 16));

        std::byte in_byte {};

        auto os     = log::info();
        auto result = device_type<"/dev/i2c@3">::read(dev_address, reg_address, std::span{std::addressof(in_byte), 1}, 100ms);

        os << "Read I2C"sv    << log::format::hex << log::format::showbase << log::endl
           << " device:   "sv << static_cast<std::uint8_t>(dev_address)    << log::endl
           << " register: "sv << static_cast<std::uint8_t>(reg_address)    << log::endl;

        if ( result == 0u )
        {
            os << " byte:     (read timeout)"sv << log::endl;
            return;
        }

        os << " byte:     "sv << static_cast<std::uint8_t>(in_byte) << log::endl;
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Read from I2C device <port 1-3> <address> <register>"sv;
    }

    static consteval auto name() { return std::string_view{"i2c-read"}; }
};

} // namespace sys

