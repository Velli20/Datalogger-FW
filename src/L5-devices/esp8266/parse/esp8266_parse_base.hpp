#pragma once

namespace sys::dev::esp8266::parse {

template <typename StateEnum>
struct base
{
    private:

    static constexpr auto k_max = 128u;

    protected:

    constexpr auto next_state()
    {
        m_index = 0;

        if ( m_state != StateEnum::k_invalid )
        {
            m_state = static_cast<StateEnum>(static_cast<std::uint32_t>(m_state) + 1);
        }
    }

    template <typename T> requires(std::is_same<T, esp8266::ip_address_type>::value)
    constexpr auto apply(T& field)
    {
        char* end = m_buffer.data();

        for ( std::size_t i{}; i < 4; ++i )
        {
            auto u = std::strtoul(end, std::addressof(end), 10);
            if ( u >= std::numeric_limits<std::uint32_t>::max() || (i < 3 && *end != '.') )
            {
                return false;
            }

            field.m_address[i] = static_cast<std::uint16_t>(u);

            if ( *end < '0' or *end > '9' )
            {
                end++;
            }
        }

        return true;
    }

    template <typename T> requires(std::is_same<T, esp8266::mac_address>::value)
    constexpr auto apply(T& field)
    {
        char* end = m_buffer.data();

        for ( std::size_t i{}; i < 6; ++i )
        {
            auto u = std::strtoul(end, std::addressof(end), 16);
            if ( u >= std::numeric_limits<std::uint32_t>::max() || (i < 5 && *end != ':') )
            {
                return false;
            }

            field.m_address[i] = static_cast<std::int16_t>(u);
            end++;
        }

        return true;
    }

    template <typename T> requires(std::is_unsigned<T>::value && !std::is_same<T, bool>::value)
    constexpr auto apply(T& field)
    {
        char* end;

        auto u = std::strtoul(m_buffer.data(), std::addressof(end), 10);
        if ( u >= std::numeric_limits<std::uint32_t>::max() )
        {
            return false;
        }

        field = static_cast<T>(u);

        return true;
    }

    template <typename T> requires(std::is_signed<T>::value && !std::is_same<T, bool>::value)
    constexpr auto apply(T& field)
    {
        char* end;

        auto u = std::strtol(m_buffer.data(), std::addressof(end), 10);
        if ( u <= std::numeric_limits<std::int32_t>::min() || u >= std::numeric_limits<std::int32_t>::max() )
        {
            return false;
        }

        field = static_cast<T>(u);

        return true;
    }

    template <typename T, typename U = typename std::underlying_type<T>::type> requires(std::is_enum<T>::value)
    constexpr auto apply(T& field)
    {
        U temp;

        if ( apply(temp) )
        {
            field = static_cast<T>(temp);
            return true;
        }

        return false;
    }

    template <typename T> requires(std::is_same<T, bool>::value)
    constexpr auto apply(T& field)
    {
        std::uint32_t temp;

        if ( apply(temp)  )
        {
            field = temp == 0 ? false : true;
            return true;
        }

        return false;
    }

    template <std::size_t N>
    constexpr auto apply(char (&s)[N])
    {
        if ( m_index < N )
        {
            std::copy_n(std::begin(m_buffer), m_index + 1, std::begin(s));
            s[m_index] = 0;
            return true;
        }

        return false;
    }

    constexpr auto buffer_append(char c)
    {
        if ( m_index >= k_max )
            return false;

        m_buffer[m_index] = c;
        m_index          += 1;

        return true;
    }

    public:

    auto begin() const
    {
        return m_buffer.cbegin();
    }

    auto end() const
    {
        return m_buffer.cend();
    }

    protected:

    StateEnum               m_state{};
    std::size_t             m_index{};
    std::array<char, k_max> m_buffer{};
};

} // namespace sys::dev::esp8266::parse
