#pragma once

namespace sys::http {

enum class mime_type : std::uint32_t
{
    k_htm,
    k_html,
    k_php,
    k_css,
    k_txt,
    k_js,
    k_json,
    k_xml,
    k_swf,
    k_flv,
    k_png,
    k_jpe,
    k_jpeg,
    k_jpg,
    k_gif,
    k_bmp,
    k_ico,
    k_tiff,
    k_tif,
    k_svg,
    k_svgz
};

namespace detail {

constexpr std::tuple<http::mime_type, std::string_view> k_mime_map[] =
{
    { mime_type::k_htm,         std::string_view{"text/html"}                       },
    { mime_type::k_html,        std::string_view{"text/html"}                       },
    { mime_type::k_php,         std::string_view{"text/html"}                       },
    { mime_type::k_css,         std::string_view{"text/css"}                        },
    { mime_type::k_txt,         std::string_view{"text/plain"}                      },
    { mime_type::k_js,          std::string_view{"application/javascript"}          },
    { mime_type::k_json,        std::string_view{"application/json"}                },
    { mime_type::k_xml,         std::string_view{"application/xml"}                 },
    { mime_type::k_swf,         std::string_view{"application/x-shockwave-flash"}   },
    { mime_type::k_flv,         std::string_view{"video/x-flv"}                     },
    { mime_type::k_png,         std::string_view{"image/png"}                       },
    { mime_type::k_jpe,         std::string_view{"image/jpeg"}                      },
    { mime_type::k_jpeg,        std::string_view{"image/jpeg"}                      },
    { mime_type::k_jpg,         std::string_view{"image/jpeg"}                      },
    { mime_type::k_gif,         std::string_view{"image/gif"}                       },
    { mime_type::k_bmp,         std::string_view{"image/bmp"}                       },
    { mime_type::k_ico,         std::string_view{"image/vnd.microsoft.icon"}        },
    { mime_type::k_tiff,        std::string_view{"image/tiff"}                      },
    { mime_type::k_tif,         std::string_view{"image/tiff"}                      },
    { mime_type::k_svg,         std::string_view{"image/svg+xml"}                   },
    { mime_type::k_svgz,        std::string_view{"image/svg+xml"}                   },
};

} // namespace detail

constexpr auto mime_to_string(http::mime_type mime)
{
    for ( auto [k_mime, k_message] : detail::k_mime_map )
    {
        if ( k_mime == mime )
        {
            return k_message;
        }
    }

    return std::string_view{"application/text"};
}

constexpr auto mime_from_string(std::string_view mime) -> std::optional<http::mime_type>
{
    for ( auto [k_mime, k_message] : detail::k_mime_map )
    {
        if ( k_message == mime )
        {
            return k_mime;
        }
    }

    return {};
}

} // namespace sys::http
