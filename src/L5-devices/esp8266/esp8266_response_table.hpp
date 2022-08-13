#pragma once

namespace sys::dev::esp8266 {

struct response_table
{
    private:

    static constexpr dev::response<esp8266::atcode_type> k_all_responses[] =
    {
        { esp8266::atcode_type::k_send_ok,                   std::string_view{"SEND OK\r\n"}                     },
        { esp8266::atcode_type::k_ok,                        std::string_view{"OK\r\n"}                          },
        { esp8266::atcode_type::k_ready_to_write,            std::string_view{"> "}                              },
        { esp8266::atcode_type::k_fail,                      std::string_view{"FAIL\r\n"}                        },
        { esp8266::atcode_type::k_dns_fail,                  std::string_view{"DNS Fail\r\n"}                    },
        { esp8266::atcode_type::k_error,                     std::string_view{"ERROR\r\n"}                       },
        { esp8266::atcode_type::k_ready,                     std::string_view{"ready\r\n"}                       },
        { esp8266::atcode_type::k_send_fail,                 std::string_view{"SEND FAIL\r\n"}                   },
        { esp8266::atcode_type::k_connected,                 std::string_view{"WIFI CONNECTED\r\n"}              },
        { esp8266::atcode_type::k_disconnected,              std::string_view{"WIFI DISCONNECT\r\n"}             },
        { esp8266::atcode_type::k_got_ip,                    std::string_view{"WIFI GOT IP\r\n"}                 },
        { esp8266::atcode_type::k_got_ip_v6,                 std::string_view{"WIFI GOT IPv6 GL\r\n"}            },
        { esp8266::atcode_type::k_busy_sending,              std::string_view{"busy s...\r\n"}                   },
        { esp8266::atcode_type::k_busy_processing,           std::string_view{"busy p...\r\n"}                   },

        { esp8266::atcode_type::k_network_connected,         std::string_view{"4,CONNECT\r\n"}                   },
        { esp8266::atcode_type::k_network_connected,         std::string_view{"3,CONNECT\r\n"}                   },
        { esp8266::atcode_type::k_network_connected,         std::string_view{"2,CONNECT\r\n"}                   },
        { esp8266::atcode_type::k_network_connected,         std::string_view{"1,CONNECT\r\n"}                   },
        { esp8266::atcode_type::k_network_connected,         std::string_view{"0,CONNECT\r\n"}                   },

        { esp8266::atcode_type::k_network_disconnected,      std::string_view{"0,CLOSED\r\n"}                    },
        { esp8266::atcode_type::k_network_disconnected,      std::string_view{"1,CLOSED\r\n"}                    },
        { esp8266::atcode_type::k_network_disconnected,      std::string_view{"2,CLOSED\r\n"}                    },
        { esp8266::atcode_type::k_network_disconnected,      std::string_view{"3,CLOSED\r\n"}                    },
        { esp8266::atcode_type::k_network_disconnected,      std::string_view{"4,CLOSED\r\n"}                    },

        { esp8266::atcode_type::k_network_already_connected, std::string_view{"ALREADY CONNECTED\r\n"}           },
        { esp8266::atcode_type::k_incoming_packet_data,      std::string_view{"+IPD,"}                           },
        { esp8266::atcode_type::k_receive_data,              std::string_view{"+CIPRECVDATA,"}                   },
        { esp8266::atcode_type::k_sntp_time,                 std::string_view{"+CIPSNTPTIME:"}                   },
        { esp8266::atcode_type::k_station_connected,         std::string_view{"+STA_CONNECTED:"}                 },
        { esp8266::atcode_type::k_station_disonnected,       std::string_view{"+STA_DISCONNECTED:"}              },
        { esp8266::atcode_type::k_station_ip_distributed,    std::string_view{"+DIST_STA_IP:"}                   },

        { esp8266::atcode_type::k_quaery_access_point,       std::string_view{"+CWLAP:"}                         },
        { esp8266::atcode_type::k_query_station_ip,          std::string_view{"+CIFSR:STAIP"}                    },
        { esp8266::atcode_type::k_query_accesspoint_ip,      std::string_view{"+CIFSR:APIP"}                     },
        { esp8266::atcode_type::k_query_station_mac,         std::string_view{"+CIFSR:STAMAC"}                   },
        { esp8266::atcode_type::k_query_accesspoint_mac,     std::string_view{"+CIFSR:APMAC"}                    },
        { esp8266::atcode_type::k_query_dns,                 std::string_view{"+CIPDNS_DEF:"}                    },
        { esp8266::atcode_type::k_query_domain,              std::string_view{"+CIPDOMAIN:"}                     },
        { esp8266::atcode_type::k_query_max_connections,     std::string_view{"+CIPSERVERMAXCONN:"}              },
        { esp8266::atcode_type::k_query_station_clients,     std::string_view{"+CWLIF:"}                         },
        { esp8266::atcode_type::k_query_available_data,      std::string_view{"+CIPRECVLEN"}                     },
    };

    template <esp8266::atcode_type to_find>
    struct find_response
    {
        static constexpr auto value = []
        {
            for ( auto c : k_all_responses )
            {
                if ( c.code() == to_find )
                {
                    return c;
                }
            }
        }();

        static constexpr auto key = std::integral_constant<esp8266::atcode_type, value.code() >{};
    };

    public:

    template <esp8266::atcode_type... k_expected_responses>
    struct make
    {
        private:

        static constexpr auto count = sizeof...(k_expected_responses);
        static constexpr auto table = std::array { find_response<k_expected_responses>::value... };

        public:

        static constexpr auto value = std::span { std::cbegin(table), count };
    };

    struct any
    {
        private:

        static constexpr auto count = sizeof(k_all_responses) / sizeof(dev::response<esp8266::atcode_type>);

        public:

        static constexpr auto value = std::span{ std::cbegin(k_all_responses), count };
    };
};

} // namespace sys::dev::esp8266

