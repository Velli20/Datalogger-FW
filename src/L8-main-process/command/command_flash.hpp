#pragma once

namespace sys {

template <>
struct shell::command_type<"flash-read"> final
{
    static constexpr auto k_page_size = 256;

    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        auto page   = static_cast<std::size_t>(std::atoi(args[0].data()));
        auto buffer = std::array<std::byte, k_page_size>{};

        auto bytes_read = device_type<"/dev/disk@1">::read(std::span{buffer}, static_cast<std::uint32_t>(page * k_page_size));
        if ( bytes_read != buffer.size() )
        {
            os << "Failed to read page "sv << page << " content"sv << log::endl;
            return;
        }

        os << "Page "sv << page << " Data: "sv << log::endl;

        log::hexdump(buffer);
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Read flash page <page>"sv;
    }
};

template <>
struct shell::command_type<"flash-mmap"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        if ( hal::peripheral_type<"/chip/octospi@1">::memorymapped_mode(true) != true )
        {
            os << "Failed to enable memorymapped mode "sv << log::endl;
            return;
        }

        //std::uint32_t m_index{0x90000000UL}; // 0x70000000UL

        log::hexdump(std::span<const std::uint32_t>{std::bit_cast<const std::uint32_t*>(0x90000000UL), 512});
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Test memory mapped mode"sv;
    }
};

template <>
struct shell::command_type<"flash-write"> final
{
    static constexpr auto k_page_size = 256;

    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        if ( args.empty() )
        {
            os << "Invalid arguments"sv << log::endl;
            return;
        }

        auto page   = static_cast<std::size_t>(std::atoi(args[0].data()));
        auto buffer = std::array<std::byte, k_page_size>{};

        os << "Type data (ESC to finish)"sv << log::endl;

        std::size_t i{};

        for ( ; i != k_page_size; ++i )
        {
            std::byte b{};

            if ( device_type<"/dev/serial@1">::read(std::span{std::addressof(b), 1}) == 0u || b == std::byte{0x1B} )
            {
                break;
            }

            os << std::to_integer<char>(b);

            buffer[i] = b;
        }

        os << log::endl << "Writing "sv << i << " on page "sv << page << log::endl;

        auto bytes_written = device_type<"/dev/disk@1">::write(std::span<const std::byte>{buffer.data(), i}, static_cast<std::uint32_t>(page * k_page_size));
        if ( bytes_written != i )
        {
            os << "Failed to write on page "sv << page << log::endl;
            return;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Write flash page <page>"sv;
    }
};

template <>
struct shell::command_type<"flash-erase"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace sys::dev;
        using namespace std::string_view_literals;

        if ( args.empty() )
        {
            os << "Invalid arguments"sv << log::endl;
            return;
        }

        auto sector = static_cast<std::uint32_t>(std::atoi(args[0].data()));

        os << log::endl << "Erasing sector "sv << sector << log::endl;

        if ( device_type<"/dev/disk@1">::erase_sector(sector) != true )
        {
            os << log::endl << "Failed to erase sector "sv << sector << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Erase flash sector <sector>"sv;
    }
};

} // namespace sys

