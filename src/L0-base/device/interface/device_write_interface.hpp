#pragma once

namespace sys::dev::interface {

namespace concepts {

template <typename F>
concept writable = requires(F fun)
{
    { fun(std::span<const std::byte>{}) } -> std::same_as<std::size_t>;
};

} // namespace concept

struct write
{
    using function_type = std::size_t (*)(std::span<const std::byte>);

    constexpr auto operator()(std::span<const std::byte>&& buffer) const  noexcept
    {
        return m_fun(std::forward<decltype(buffer)>(buffer));
    }

    static consteval auto make(concepts::writable auto&& fun)
    {
        return dev::interface::write{std::move(fun)};
    }

    const function_type m_fun;
};

struct read
{
    using function_type = std::size_t (*)(std::span<std::byte>);

    constexpr auto operator()(std::span<std::byte>&& buffer) const noexcept
    {
        return m_fun(std::forward<decltype(buffer)>(buffer));
    }

    const function_type m_fun;
};

using driverf = std::variant<
    std::monostate,
    dev::interface::write,
    dev::interface::read
>;

} // namespace sys::dev::interface
