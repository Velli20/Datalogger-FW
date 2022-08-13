#pragma once

namespace sys::dev::esp8266 {

struct ap_info_type
{
    esp8266::encryption_type m_encryption;
    char                     m_ssid[128];
    std::int32_t             m_signal_strength;
    esp8266::mac_address     m_mac;
    std::int32_t             m_channel;
    std::int32_t             m_frequency_offset;
    std::int32_t             m_frequency_calibration;
    esp8266::chiper_type          m_pairwise_chiper;
    esp8266::chiper_type          m_group_chiper;
    esp8266::wifi_mode       m_wifi_mode;
    bool                     m_wps_enabled;
};

} // namespace sys::dev::esp8266


namespace sys::log {

template <class T>
constexpr auto operator<<(
    formatting_stream<T>&             os,
    const dev::esp8266::ap_info_type& ap
) noexcept -> formatting_stream<T>&
{
    using namespace sys::dev;
    using namespace std::string_view_literals;

    os << "Encryption:            "sv;

    switch ( ap.m_encryption )
    {
        case esp8266::encryption_type::k_open:
            os << "None"sv;
            break;

        case esp8266::encryption_type::k_wep:
            os << "WEP"sv;
            break;

        case esp8266::encryption_type::k_wpa_psk:
            os << "WPA PSK"sv;
            break;

        case esp8266::encryption_type::k_wpa2_psk:
            os << "WPA2 PSK"sv;
            break;

        case esp8266::encryption_type::k_wpa_wap2_psk:
            os << "WPA WAP2 PSK"sv;
            break;

        case esp8266::encryption_type::k_wpa_enterprise:
            os << "WPA Enterprise"sv;
            break;
    }

    os << log::endl;

    const auto print_chiper = [&os] (esp8266::chiper_type c)
    {
        using namespace std::string_view_literals;

        switch ( c )
        {
            case esp8266::chiper_type::k_none:
                os << "None"sv;
                break;

            case esp8266::chiper_type::k_wep40:
                os << "WEP40"sv;
                break;

            case esp8266::chiper_type::k_wep104:
                os << "WEP104"sv;
                break;

            case esp8266::chiper_type::k_tkip:
                os << "TKIP"sv;
                break;

            case esp8266::chiper_type::k_ccmp:
                os << "CCMP"sv;
                break;

            case esp8266::chiper_type::k_tkip_ccmp:
                os << "TKIP CCMP"sv;
                break;

            case esp8266::chiper_type::k_unkown:
                os << "Unknown"sv;
                break;
        }
    };

    os << "SSID:                  "sv << ap.m_ssid                  << log::endl;
    os << "signal strength:       "sv << ap.m_signal_strength       << log::endl;
    os << "MAC-address:           "sv << ap.m_mac                   << log::endl;
    os << "Channel:               "sv << ap.m_channel               << log::endl;
    os << "Frequency offset:      "sv << ap.m_frequency_offset      << log::endl;
    os << "Frequency Calibration: "sv << ap.m_frequency_calibration << log::endl;

    os << "Pairwise chiper_type:       "sv;
    print_chiper(ap.m_pairwise_chiper);
    os << log::endl;

    os << "Group chiper_type:          "sv;
    print_chiper(ap.m_group_chiper);
    os << log::endl;

    os << "Mode:                  "sv;

    switch ( ap.m_wifi_mode )
    {
        case esp8266::wifi_mode::kModeB:
            os << "B"sv;
            break;

        case esp8266::wifi_mode::kModeG:
            os << "G"sv;
            break;

        case esp8266::wifi_mode::kModeN:
            os << "N"sv;
            break;

        case esp8266::wifi_mode::kModeBg:
            os << "BG"sv;
            break;

        case esp8266::wifi_mode::kModeBgn:
            os << "BGN"sv;
            break;

        default:
            os << "Unknown"sv;
    }

    os << log::endl;
    os << "WPS enabled:           "sv << ap.m_wps_enabled;

    return os;
}

} // namespace sys::log
