#pragma once

namespace sys::dev::esp8266::parse {

namespace detail {

enum class ap_state : std::uint32_t
{
    kParseEncryption = 0,
    kParseSsid,
    kParseRssi,
    kParseMac,
    kParseChannel,
    kParseFrequencyOffset,
    kParseFrequencyCalibration,
    kParsePairwiseCipher,
    kParseGroupCipher,
    kParseMode,
    kParseWps,
    k_invalid,
    k_max = k_invalid
};

} // namespace detail

struct access_point : public parse::base<detail::ap_state>
{
    /* constexpr */ explicit access_point(esp8266::ap_info_type& ap) : m_ap{ap}{}

    // <ecn>,<m_ssid>,<rssi>,<m_mac>,<m_channel>,<freqoffset>,<freqcali>,<pairwise_cipher>, <group_cipher>,<bgn>,<wps>

    private:

    using parse::base<detail::ap_state>::apply;
    using parse::base<detail::ap_state>::buffer_append;
    using parse::base<detail::ap_state>::next_state;

    [[nodiscard]] constexpr auto Get() -> esp8266::ap_info_type&
    {
        return m_ap.get();
    }

    [[nodiscard]] constexpr auto do_parse()
    {
        switch ( m_state )
        {
            case detail::ap_state::kParseEncryption:
                return apply(Get().m_encryption);

            case detail::ap_state::kParseSsid:
                return apply(Get().m_ssid);

            case detail::ap_state::kParseRssi:
                return apply(Get().m_signal_strength);

            case detail::ap_state::kParseMac:
                return apply(Get().m_mac);;

            case detail::ap_state::kParseChannel:
                return apply(Get().m_channel);

            case detail::ap_state::kParseFrequencyOffset:
                return apply(Get().m_frequency_offset);

            case detail::ap_state::kParseFrequencyCalibration:
                return apply(Get().m_frequency_calibration);

            case detail::ap_state::kParsePairwiseCipher:
                return apply(Get().m_pairwise_chiper);

            case detail::ap_state::kParseGroupCipher:
                return apply(Get().m_group_chiper);

            case detail::ap_state::kParseMode:
                return apply(Get().m_wifi_mode);

            case detail::ap_state::kParseWps:
                return apply(Get().m_wps_enabled);

            case detail::ap_state::k_invalid:
                return false;
        }

        return true;
    }

    public:

    [[maybe_unused]] constexpr auto operator()(char c)
    {
        if ( c == '\r' or c == '\n' )
            return false;

        if ( c == '"' )
            return true;

        if ( c == ')' )
            return false;

        if ( c == '(' )
        {
            m_state = detail::ap_state::kParseEncryption;
            m_index = 0;
        }

        else if ( c == ',' || c == ')' )
        {
            m_buffer.data()[m_index] = 0;

            if ( do_parse() == false )
                return false;

            next_state();

            return true;
        }

        return buffer_append(c);
    }

    private:

    std::reference_wrapper<esp8266::ap_info_type> m_ap;
};

} // namespace sys::dev::esp8266::parse
