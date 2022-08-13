#pragma once

namespace sys::dev::esp8266 {

struct unsolicited_response
{
    constexpr unsolicited_response(esp8266::atcode_type code)
        : m_code{code}
        {}

    constexpr unsolicited_response(
        esp8266::atcode_type code,
        auto &&...           others
    )
        : m_code{code}
        , m_data{static_cast<std::uint32_t>(others)...}
        {}

    [[nodiscard]] constexpr auto code() const
    {
        return m_code;
    }

    [[nodiscard]] constexpr auto data(std::size_t i) const
    {
        return m_data[i];
    }

    esp8266::atcode_type         m_code{};
    std::array<std::uint32_t, 4> m_data{};
};

} // namespace sys::dev::esp8266
