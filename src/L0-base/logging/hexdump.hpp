#pragma once

namespace sys::log {

namespace {

template <typename InputIterator>
inline auto hexdump(
    InputIterator       begin,
    const InputIterator end
)
{
    static constexpr auto k_bits_per_byte         = std::uint8_t{8u};
    static constexpr auto k_printable_ascii_range = std::pair<std::uint8_t, std::uint8_t>{32u, 126u};
    static constexpr auto k_bytes_per_row         = std::uint8_t{k_bits_per_byte * 2u};

    auto offset = std::uint32_t{};
    auto first  = true;
    auto os     = log::formatting_stream{io::stdio_stream_sink{}};

    os << log::format::hex;

    do
    {
        if ( first )
        {
            first = false;
        }

        else
        {
            os << log::endl;
        }

        os << offset << "  ";

        offset += k_bytes_per_row;

        auto it = begin;
        for ( std::uint8_t i{}; i < k_bytes_per_row; ++i )
        {
            if ( i == k_bits_per_byte )
            {
                os << ' ';
            }

            if ( it != end )
            {
                os << static_cast<std::uint8_t>(*it) << ' ';
                ++it;
            }

            else
            {
                os << "   ";
            }
        }

        os << " ";

        for ( std::uint8_t i{}; i != k_bytes_per_row; ++i )
        {
            if ( begin != end )
            {
                os << (((static_cast<std::uint32_t>(*begin) >= k_printable_ascii_range.first) &&
                        (static_cast<std::uint32_t>(*begin) <= k_printable_ascii_range.second))
                            ? static_cast<char>(*begin)
                            : '.');
                ++begin;
            }

            else
            {
                os << ' ';
            }
        }
    }
    while ( begin != end );

    os << log::endl;
}

} // namespace

template <typename Container>
inline auto hexdump(const Container& cont)
{
    return hexdump(std::begin(cont), std::end(cont));
}

} // namespace sys::log
