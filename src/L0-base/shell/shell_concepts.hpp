#pragma once

namespace sys::shell::concepts {

template <string::static_string k_command>
concept has_usage = requires()
{
    { shell::command_type<k_command>::description() } -> std::same_as<std::string_view>;
};

template <string::static_string k_command>
concept has_argument_completion_Callback = requires()
{
    shell::command_type<k_command>::on_try_complete_argument(std::string_view{}, std::span<char>{});
};

template <string::static_string k_command>
concept has_match_callback = requires()
{
    std::is_same<decltype(shell::command_type<k_command>::on_command_match), shell::command_view::function_type_1>::value ||
    std::is_same<decltype(shell::command_type<k_command>::on_command_match), shell::command_view::function_type_2>::value ||
    std::is_same<decltype(shell::command_type<k_command>::on_command_match), shell::command_view::function_type_3>::value;
};

} // namespace sys::shell::concepts
