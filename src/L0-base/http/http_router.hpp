
namespace sys::http {

struct router_interface
{
    [[nodiscard]] virtual std::optional<http::router_response_type> operator()(const http::message& req) const = 0;
};

namespace detail {

[[nodiscard]] static auto try_route(
    const http::message&                req,
    sys::concepts::container_like auto& nodes
) noexcept -> std::optional<http::router_response_type>
{
    for ( auto& n : nodes )
    {
        if ( req.url() == n.m_path )
        {
            return std::invoke(n.m_callback, req);
        }
    }

    return {};
}

} // namespace detail

struct node
{
    using function_type = http::router_response_type (*)(const http::message&);

    constexpr node() = default;

    consteval node(
        std::string_view path,
        function_type    fun
    )
        : m_path{path}
        , m_callback{fun}
    {}

    std::string_view m_path{};
    function_type    m_callback{};
};

template <std::size_t k_max = 0>
struct router : public std::array<node, k_max>, http::router_interface
{
    public:

    using base = std::array<node, k_max>;

    constexpr router() {}

    template <http::method_type, string::static_string k_path>
    [[nodiscard]] consteval auto on(concepts::router_callback auto callback)
    {
        http::router<k_max + 1> new_server{};

        std::copy_n(std::begin(*this), k_max, std::begin(new_server));

        new_server[k_max].m_path     = k_path;
        new_server[k_max].m_callback = callback;

        return new_server;
    }

    static consteval auto make()
    {
        return http::router<>{};
    }

    [[nodiscard]] std::optional<http::router_response_type> operator()(const http::message& req) const override
    {
        return detail::try_route(req, *this);
    }

    using base::data;
    using base::operator[];
    using base::at;

    private:

    using base::cbegin;
    using base::cend;
    using base::rbegin;
    using base::rend;
};

template <class k_server, sys::string::static_string... k_path>
consteval auto make_router()
{
    struct router<sizeof...(k_path)> router{};

    [&] <std::size_t... Is> (std::integer_sequence<std::size_t, Is...>)
    {
        ([&]
        {
            constexpr auto k_path_n = std::get<Is>(std::forward_as_tuple(k_path...));

            router[Is] = http::node { std::string_view{k_path}, k_server::template on<k_path_n> };
        }(), ...);

    }(std::make_integer_sequence<std::size_t, sizeof...(k_path)>{});

    return router;
}

} // namespace sys::http
