#pragma once

namespace sys::dev::esp8266 {

enum class atcode_type : std::uint32_t
{
    k_ok = 0,
    k_ready_to_write,
    k_fail,
    k_dns_fail,
    k_error,
    k_ready,
    k_send_ok,
    k_send_fail,
    k_connected,
    k_receive_data,
    k_got_ip,
    k_got_ip_v6,
    k_disconnected,
    k_busy_sending,
    k_busy_processing,
    k_network_connected,
    k_network_disconnected,
    k_network_already_connected,
    k_incoming_packet_data,
    k_timeout,
    k_quaery_access_point,
    k_sntp_time,
    k_station_connected,
    k_station_disonnected,
    k_station_ip_distributed,
    k_query_accesspoint_mac,
    k_query_max_connections,
    k_query_accesspoint_ip,
    k_query_station_ip,
    k_query_station_mac,
    k_query_dns,
    k_query_domain,
    k_query_station_clients,
    k_query_available_data,
};


} // namespace sys::dev::esp8266
