#pragma once

namespace sys::dev::esp8266::parse {

namespace detail {

template <std::size_t N>
static constexpr std::uint32_t make_hash(const char (&s)[N])
{
    return util::hash_fnv1a(s, 3);
}

static constexpr std::uint32_t make_hash(const char* s)
{
    return util::hash_fnv1a(s, 3);
}

enum class sntp_parser_state_type : std::uint32_t
{
    k_wait_begin = 0,
    k_parse_weekday,
    k_parse_month,
    k_parse_month_day,
    k_parse_hour,
    k_parse_minutes,
    k_parse_seconds,
    k_parse_year,
    k_invalid,
    k_max = k_invalid
};

} // namespace detail

struct sntp : private parse::base<detail::sntp_parser_state_type>
{
    static constexpr std::tuple<std::uint32_t, int> k_week_days[] =
    {
        { detail::make_hash("Mon"), 0 },
        { detail::make_hash("Tue"), 1 },
        { detail::make_hash("Wed"), 2 },
        { detail::make_hash("Thu"), 3 },
        { detail::make_hash("Fri"), 4 },
        { detail::make_hash("Sat"), 5 },
        { detail::make_hash("Sun"), 6 }
    };

    static constexpr std::tuple<std::uint32_t, int> k_month_days[] =
    {
        { detail::make_hash("Jan"), 0  },
        { detail::make_hash("Feb"), 1  },
        { detail::make_hash("Mar"), 2  },
        { detail::make_hash("Apr"), 3  },
        { detail::make_hash("May"), 4  },
        { detail::make_hash("Jun"), 5  },
        { detail::make_hash("Jul"), 6  },
        { detail::make_hash("Aug"), 7  },
        { detail::make_hash("Sep"), 8  },
        { detail::make_hash("Oct"), 9  },
        { detail::make_hash("Nov"), 10 },
        { detail::make_hash("Dec"), 11 }
    };

    constexpr explicit sntp(std::tm& t)
        : m_time{t}{}

    // Thu Jan 01 00:00:00 1970OK

    private:

    using parse::base<detail::sntp_parser_state_type>::apply;

    [[nodiscard]] constexpr auto Get() -> std::tm&
    {
        return m_time.get();
    }

    template <detail::sntp_parser_state_type kState> requires(kState == detail::sntp_parser_state_type::k_parse_weekday)
    constexpr auto apply(std::tm& field)
    {
        const auto weekday = detail::make_hash(m_buffer.data());
        for ( auto [literal, number] : k_week_days )
        {
            if ( literal == weekday )
            {
                field.tm_wday = number;
                return true;
            }
        }

        return false;
    }

    template <detail::sntp_parser_state_type kState> requires(kState == detail::sntp_parser_state_type::k_parse_month)
    constexpr auto apply(std::tm& field)
    {
        const auto n = detail::make_hash(m_buffer.data());
        for ( auto [literal, number] : k_month_days )
        {
            if ( literal == n )
            {
                field.tm_mon = number;
                return true;
            }
        }

        return false;
    }

    template <detail::sntp_parser_state_type kState> requires(kState == detail::sntp_parser_state_type::k_parse_year)
    constexpr auto apply(std::tm& field)
    {
        std::int32_t tmp;

        if ( auto rc = apply(tmp); rc == true )
        {
            field.tm_year = (tmp - 1900);
            return true;
        }

        return false;
    }

    [[maybe_unused]] constexpr auto do_parse()
    {
        switch ( m_state )
        {
            case detail::sntp_parser_state_type::k_parse_weekday:
                return apply<detail::sntp_parser_state_type::k_parse_weekday>(Get());

            case detail::sntp_parser_state_type::k_parse_month:
                return apply<detail::sntp_parser_state_type::k_parse_month>(Get());

            case detail::sntp_parser_state_type::k_parse_month_day:
                return apply(Get().tm_mday);

            case detail::sntp_parser_state_type::k_parse_hour:
                return apply(Get().tm_hour);

            case detail::sntp_parser_state_type::k_parse_minutes:
                return apply(Get().tm_min);

            case detail::sntp_parser_state_type::k_parse_seconds:
                return apply(Get().tm_sec);

            case detail::sntp_parser_state_type::k_parse_year:
                return apply<detail::sntp_parser_state_type::k_parse_year>(Get());

            case detail::sntp_parser_state_type::k_invalid:
                [[fallthrough]];

            case detail::sntp_parser_state_type::k_wait_begin:
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

        if ( m_index >= m_buffer.size() )
        {
            return false;
        }

        if ( /* c == ':' &&  */m_state == detail::sntp_parser_state_type::k_wait_begin )
        {
            m_index = 0;
            next_state();
            return true;
        }

        if ( c == ' ' || c == ':' )
        {
            m_buffer.data()[m_index] = 0;
            do_parse();
            next_state();
        }

        else
        {
            buffer_append(c);
        }

        if ( m_state == detail::sntp_parser_state_type::k_parse_year and m_index == 4 )
        {
            do_parse();
            return false;
        }

        return true;
    }

    private:

    std::reference_wrapper<std::tm> m_time;
};

} // namespace sys::dev::esp8266::parse
