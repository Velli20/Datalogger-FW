#pragma once

namespace sys {

template <>
struct shell::command_type<"hv-channel-read"> final
{
    static constexpr auto kTestByte = std::array<std::byte, 1>{};

    static auto on_command_match(const shell::arguments& args)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        auto os = log::info();

        os << log::endl << "CHANNEL  HIGH   LOW"sv << log::endl;

        constexpr readable_type k_ios[]
        {
            device_type<"/dev/24-dc-input@1">::get_readable(),
            device_type<"/dev/24-dc-input@2">::get_readable(),
            device_type<"/dev/24-dc-input@3">::get_readable(),
            device_type<"/dev/24-dc-input@4">::get_readable(),
            device_type<"/dev/24-dc-input@5">::get_readable(),
            device_type<"/dev/24-dc-input@6">::get_readable(),
            device_type<"/dev/24-dc-input@7">::get_readable(),
            device_type<"/dev/24-dc-input@8">::get_readable()
        };

        for ( std::size_t i{}; i < 8; ++i )
        {
            os << static_cast<std::uint8_t>(i) << ":"sv;

            if ( std::invoke(k_ios[i]) )
            {
                os << "        [x]   [ ]"sv;
            }

            else
            {
                os << "        [ ]   [x]"sv;
            }

            os << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Read 24 DC I/O Channel <channel 1-8>"sv;
    }
};


} // namespace sys

