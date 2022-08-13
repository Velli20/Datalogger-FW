#pragma once

namespace sys::http {

struct file
{
    explicit constexpr file(std::string_view s)
        : m_path{s}
        {}

    [[nodiscard]] auto path() const
    {
        return std::filesystem::path{m_path};
    }

    std::string_view m_path{};
};

using response_body = std::variant<
    std::monostate,
    std::string_view,
    platform::pmr::string,
    std::filesystem::path,
    io::input_sink_iterator
>;

} // namespace sys::http
