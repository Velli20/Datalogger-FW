#pragma once

namespace sys::dev::esp8266::parse {

namespace detail {

enum class ip_state : std::uint32_t
{
    k_none,
    k_parsing
};

} // namespace detail

struct ip : public parse::base<detail::ip_state>
{
    constexpr explicit ip(esp8266::ip_address_type& v)
        : m_ip{v}{}

    private:

    using parse::base<detail::ip_state>::apply;
    using parse::base<detail::ip_state>::buffer_append;

    public:

    [[maybe_unused]] constexpr auto operator()(char c)
    {
        if ( m_state != detail::ip_state::k_parsing && c == '"' )
        {
            m_state = detail::ip_state::k_parsing;
            return true;
        }

        else if ( c == '\r' )
        {
            if ( m_state == detail::ip_state::k_parsing )
            {
                m_buffer.data()[m_index + 1] = 0;

                apply(m_ip.get());
                return false;
            }

            return false;
        }

        if ( m_state == detail::ip_state::k_parsing )
        {
            if ( (c >= '0' and c <= '9') or c == '.' or c == '"' )
            {
                return buffer_append(c);
            }
        }

        return true;
    }

    private:

    std::reference_wrapper<esp8266::ip_address_type> m_ip;
};

} // namespace sys::dev::esp8266::parse
