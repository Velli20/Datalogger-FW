#pragma once

namespace sys::http::concepts {

template <typename Func>
concept router_callback = requires(Func f, const sys::http::message& r)
{
    { std::invoke(f, r) } -> std::same_as<http::router_response_type>;
};

} // namespace sys::http::concepts
