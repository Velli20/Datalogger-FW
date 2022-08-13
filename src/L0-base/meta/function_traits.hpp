#pragma once

namespace sys::meta::detail {

template <std::size_t N, typename...Args>
struct select_nth;

template <std::size_t N, typename Arg0, typename...Args>
struct select_nth<N,Arg0,Args...> : select_nth<N-1,Args...>{};

template <std::size_t N>
struct select_nth<N>{}; // empty, to allow for sfinae

template <typename Arg0, typename...Args>
struct select_nth<0,Arg0,Args...>{ using type = Arg0; };

template <typename Fn>
struct function_type_traits;

template <typename R, typename...Args>
struct function_type_traits<R(Args...)>
{
    static constexpr auto is_noexcept = false;
    static constexpr auto arity       = sizeof...(Args);

    using result_type    = R;
    using signature_type = R(Args...);

    template <std::size_t N>
    using param_type = typename select_nth<N,Args...>::type;
};

template <typename R, typename...Args>
struct function_type_traits<R(Args...) noexcept>
{
    static constexpr bool is_noexcept = true;
    static constexpr auto arity       = sizeof...(Args);

    using result_type    = R;
    using signature_type = R(Args...) noexcept;

    template <std::size_t N>
    using param_type = typename select_nth<N,Args...>::type;
};

template <typename R, typename...Args>
struct function_type_traits<R(*)(Args...)> : function_type_traits<R(Args...)>{};

} // namespace sys::meta

namespace sys::meta {

template <auto F>
using function_traits = detail::function_type_traits<decltype(F)>;

} // namespace sys::meta
