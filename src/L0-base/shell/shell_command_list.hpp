#pragma once

namespace sys::shell {

template <string::static_string... k_commands>
static consteval auto command_list() -> std::array<shell::command_view, sizeof...(k_commands)>
{
    auto list = std::array<shell::command_view, sizeof...(k_commands)>{};

    [&] <std::size_t... Is> (std::integer_sequence<std::size_t, Is...>)
    {
        ([&]
        {
            constexpr auto k_command = std::get<Is>(std::forward_as_tuple(k_commands...));

            static_assert(shell::concepts::has_match_callback<k_command>, "Command must implement on_command_match() callback");

            shell::command_view cmd{shell::command_type<k_command>::on_command_match, std::string_view{k_commands}};

            if constexpr ( shell::concepts::has_usage<k_command> )
            {
                cmd.description(shell::command_type<k_command>::description());
            }

            if constexpr ( shell::concepts::has_argument_completion_Callback<k_command> )
            {
                cmd.autocomplete_helper(shell::command_type<k_command>::on_try_complete_argument);
            }

            list[Is] = cmd;

        }(), ...);

    }(std::make_integer_sequence<std::size_t, sizeof...(k_commands)>{});

    const auto cmd_comparator = [](const shell::command_view& lhs, const shell::command_view& rhs)
    {
        return lhs.name() < rhs.name();
    };

    std::sort(std::begin(list), std::end(list), cmd_comparator);

    return list;
}

} // namespace sys::shell
