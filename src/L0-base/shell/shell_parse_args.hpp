#pragma once

namespace sys::shell {

template <std::size_t N1, std::size_t N2>
constexpr auto split_arguments(
    std::array<char, N1>&             buffer,
    std::array<std::string_view, N2>& argv
) noexcept
{
    auto start = buffer.begin();
    auto count = std::size_t{};

    while ( count < N2 && count < buffer.size() )
    {
        start = std::find_if(start, buffer.end(), [](auto n) { return n != '\0' && n != ' ' && n != '\n' && n != '\r'; });
        if ( start == buffer.end() )
        {
            break;
        }

        auto end    = std::find_if(start, buffer.end(), [](auto n) { return n == ' ' || n == '\0' || n == '\n' || n == '\r' || n == '\t'; });
        auto length = std::distance(start, end);

        *end = '\0';
        argv[count] = std::string_view{start, static_cast<std::string_view::size_type>(length)};

        start  = end;
        count += 1;
    }

    return count;
}

} // namespace sys::shell
