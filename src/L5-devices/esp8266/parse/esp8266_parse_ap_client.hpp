#pragma once

namespace sys::dev::esp8266::parse {

namespace detail {

enum class connected_client_state : std::uint32_t
{
    parse_ip = 0,
    parse_mac,
    none,
    k_max = none,
    k_invalid = k_max
};

} // namespace detail

struct connected_client : public parse::base<detail::connected_client_state>
{
    /* constexpr */ explicit connected_client(esp8266::ip_address_type& ip, esp8266::mac_address& mac)
        : m_ip{ip}, m_mac{mac} {}

    // <ip addr>,<m_mac>

    private:

    using parse::base<detail::connected_client_state>::apply;
    using parse::base<detail::connected_client_state>::buffer_append;
    using parse::base<detail::connected_client_state>::next_state;

    [[nodiscard]] constexpr auto do_parse()
    {
        switch ( m_state )
        {
            case detail::connected_client_state::parse_ip:
                return apply(m_ip.get());

            case detail::connected_client_state::parse_mac:
                return apply(m_mac.get());

            case detail::connected_client_state::none:
                return false;
        }

        return true;
    }

    public:

    [[maybe_unused]] constexpr auto operator()(char c)
    {
        if ( c == '\r' or c == '\n' )
        {
            return false;
        }

        else if ( c == ',' )
        {
            m_buffer.data()[m_index] = 0;

            if ( do_parse() == false )
            {
                return false;
            }

            next_state();

            return true;
        }

        return buffer_append(c);
    }

    private:

    std::reference_wrapper<esp8266::ip_address_type>  m_ip;
    std::reference_wrapper<esp8266::mac_address> m_mac;
};

} // namespace sys::dev::esp8266::parse
