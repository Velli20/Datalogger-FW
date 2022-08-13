#pragma once

#include <optional>
#include <concepts>

namespace sys::shell {

struct command_view
{
    enum class function_type : std::uint8_t
    {
        simple,
        with_args,
        with_prompt
    };

    using function_type_1 = void (*)();
    using function_type_2 = void (*)(const shell::arguments&);
    using function_type_3 = void (*)(const shell::arguments&, log::formatting_stream<shell::input_line>&);

    using match_callback_type = std::variant<
        function_type_1,
        function_type_2,
        function_type_3
    >;

    using autocomplete_callback_type = void (*)(std::string_view, std::span<char>);

    std::string_view           m_description{};
    std::string_view           m_name{};
    match_callback_type        m_fun{};
    autocomplete_callback_type m_auto_complete_fun{};
    function_type              type{};

    constexpr command_view() = default;

    template <typename F>
    consteval command_view(
        F                fun,
        std::string_view name = std::string_view{}
    )
        : m_name{name}
        , m_fun{fun}
        , type{function_type::simple}
        {}

    [[nodiscard]] constexpr auto name() const noexcept
    {
        return m_name;
    }

    [[nodiscard]] constexpr auto description() const noexcept
    {
        return m_description;
    }

    constexpr auto description(std::string_view d) noexcept
    {
        m_description = d;
    }

    constexpr auto autocomplete_helper(autocomplete_callback_type f) noexcept
    {
        m_auto_complete_fun = f;
    }

    [[nodiscard]] auto autocomplete_helper() const noexcept -> std::optional<autocomplete_callback_type>
    {
        if ( m_auto_complete_fun == nullptr )
        {
            return {};
        }

        return m_auto_complete_fun;
    }


    constexpr void operator()(
        const shell::arguments&                    args,
        log::formatting_stream<shell::input_line>& input_line
    ) const noexcept
    {
        std::visit(util::overloaded
        {
            [](const function_type_1 fun)  { std::invoke(fun);                   },
            [&](const function_type_2 fun) { std::invoke(fun, args);             },
            [&](const function_type_3 fun) { std::invoke(fun, args, input_line); }
        }, m_fun);
    }
};

} // namespace sys::shell
