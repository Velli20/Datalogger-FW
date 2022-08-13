
namespace sys::http {

struct url_components
{
    http::protocol_type protocol{http::protocol_type::k_http};
    std::string_view    host{};
    std::string_view    path{};
    std::uint16_t       port{80u};
};

} // namespace sys::http
