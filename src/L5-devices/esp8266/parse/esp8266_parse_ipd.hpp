#pragma once

namespace sys::dev::esp8266::parse {

namespace detail {

enum class ipd_parser_state_type : std::uint32_t
{
    k_none
};

} // namespace detail

struct ipd final : private parse::base<detail::ipd_parser_state_type>
{
    constexpr ipd() = default;

    /* Format: +IPD,c,n:xxxxxxxxxx
       Or:     +IPD,n:xxxxxxxxxx
       Or:     +IPD,n,123.123.123.123,80:xxxxxxxxxx

       Where c is connection number and n is amount of bytes to receive
    */
    // format is : +IPD,<id>,<len>:<data>
    // format is : +IPD,<ID>,<len>[,<remote IP>,<remote port>]:<data>

    private:

    using parse::base<detail::ipd_parser_state_type>::apply;

    public:

    [[maybe_unused]] constexpr auto operator()(char c)
    {
        if ( c == ',' || c == ':' || c == '\r' )
        {
            m_buffer.data()[m_index] = 0;

            // log::info() << "IPD Apply:" << log::endl << m_buffer.data() << log::endl;

            if ( m_count < 2 )
            {
                apply(m_values[m_count]);
            }

            else if ( m_count == 2 )
            {
                apply(m_ip);
            }

            else if ( m_count == 3 )
            {
                apply(m_values[2]);
            }

            m_index  = 0;
            m_count += 1;

            if ( c == ':' || c == '\r' )
                return false;
        }

        else
        {
            return buffer_append(c);
        }

        return true;
    }

    [[nodiscard]] auto connection() const noexcept -> std::optional<std::uint32_t>
    {
        if ( m_count >= 2 )
        {
            return m_values[0];
        }

        return {};
    }

    [[nodiscard]] auto port() const noexcept -> std::optional<std::uint32_t>
    {
        if ( m_count == 4 )
        {
            return m_values[2];
        }

        return {};
    }

    [[nodiscard]] constexpr auto count() const noexcept -> std::optional<std::uint32_t>
    {
        if ( m_count >= 2 )
        {
            return m_values[1];
        }

        if ( m_count == 1 )
        {
            return m_values[0];
        }

        return {};
    }

    [[nodiscard]] auto ip() const noexcept -> std::optional<esp8266::ip_address_type>
    {
        if ( m_count == 4 )
        {
            return m_ip;
        }

        return {};
    }

    private:

    std::uint8_t        m_count{};
    std::uint32_t       m_values[3]{};
    esp8266::ip_address_type m_ip{};

};

} // namespace sys::dev::esp8266::parse
