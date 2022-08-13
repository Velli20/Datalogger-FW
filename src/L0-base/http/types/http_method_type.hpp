#pragma once

namespace sys::http {

enum class method_type
{
    k_connect,
    k_delete,
    k_get,
    k_head,
    k_options,
    k_patch,
    k_post,
    k_put,
    k_trace,
    k_unknown
};

namespace detail {

constexpr std::tuple<std::string_view, http::method_type> k_methods[] =
{
    { std::string_view{"CONNECT"}, http::method_type::k_connect  },
    { std::string_view{"DELETE"},  http::method_type::k_delete   },
    { std::string_view{"GET"},     http::method_type::k_get      },
    { std::string_view{"HEAD"},    http::method_type::k_head     },
    { std::string_view{"OPTIONS"}, http::method_type::k_options  },
    { std::string_view{"PATCH"},   http::method_type::k_patch    },
    { std::string_view{"POST"},    http::method_type::k_post     },
    { std::string_view{"PUT"},     http::method_type::k_put      },
    { std::string_view{"TRACE"},   http::method_type::k_trace    },
};

} // namespace detail


consteval auto to_string(http::method_type method) -> std::string_view
{
    for ( const auto& [text, value] : detail::k_methods )
    {
        if ( method == value )
        {
            return text;
        }
    }

    return {};
}

[[nodiscard]] static auto method_from_string(std::string_view method_string) -> std::optional<http::method_type>
{
    for ( const auto& [text, value] : detail::k_methods )
    {
        if ( sys::algorithm::iequals(text, method_string) )
        {
            return value;
        }
    }

    return {};
}

} // namespace sys::http
