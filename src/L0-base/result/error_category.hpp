#pragma once

namespace sys {

template <typename T>
struct is_error_code_enum : std::false_type {};

struct error_category
{
    public:

    constexpr error_category() = default;

    ~error_category() = default;

    constexpr virtual std::string_view name() const noexcept = 0;

    constexpr virtual std::string_view message(std::int32_t code) const noexcept = 0;

    [[nodiscard]] constexpr bool success(std::int32_t code) const
    {
        return code == m_success_code;
    }

    private:

    std::int32_t m_success_code{};
};

template <typename T>
constexpr decltype(auto) category()
{}

namespace detail {

struct empty_error_category : public sys::error_category
{
    constexpr empty_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"Default"};  }

    [[nodiscard]] constexpr std::string_view message(std::int32_t) const noexcept override
    {
        return {};
    }
};

} // namespace detail

struct error_code
{
    private:

    static constexpr auto k_default_category = detail::empty_error_category{};
    public:

    constexpr error_code()
        : m_category{std::addressof(k_default_category)},
          m_code{}
        {}

    template <typename E> requires(is_error_code_enum<E>::value)
    constexpr error_code(E code, const error_category& category)
        : m_category{std::addressof(category)},
          m_code{static_cast<std::int32_t>(std::underlying_type_t<E>(code))}
        {}

    [[nodiscard]] auto category() const -> const error_category&
    {
        return *m_category;
    }

    [[nodiscard]] constexpr auto code() const
    {
        return m_code;
    }

    [[nodiscard]] auto message() const -> std::string_view
    {
        return m_category->message(m_code);
    }

    [[nodiscard]] explicit operator bool() const
    {
        return m_category->success(m_code);
    }

    template <typename T>
    friend decltype(auto) operator<<(T os, const sys::error_code& arg) noexcept
    {
        os << '[' <<(arg.category().name()) << "] Error code: ";
        os << arg.code() << " Message: ";

        if ( arg.message().empty() == false )
        {
            os << arg.message();
        }

        else
        {
            os << std::string_view{"(No error message available)"};
        }

        os << sys::log::endl;

        return os;
    }

    static constexpr std::string_view no_error{};

    private:

    const error_category* m_category{};
    std::int32_t          m_code{};
};

template <typename E>
decltype(auto) make_unexpected(E e)
{
    return tl::make_unexpected<typename sys::error_code>(make_error_code(e));
}

template <class T, class E>
using expected = tl::expected<T, E>;

} // namespace sys
