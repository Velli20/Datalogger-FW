#pragma once

namespace sys {

namespace {

template <typename T>
[[nodiscard]] constexpr auto status_test_reset(
    T     bit,
    auto& bits
) noexcept -> bool
{
    if ( bits.test(bit) == true )
    {
        bits.reset(bit);
        return true;
    }

    return false;
}

template <typename T>
[[nodiscard]] constexpr auto starts_with(
    const std::string_view command,
    const T&               buffer
) noexcept -> bool
{
    return std::strncmp(command.data(), buffer.data(), buffer.size() - 1) == 0;
}

[[nodiscard]] constexpr auto find_program(
    const std::span<const shell::command_view>& command_list,
    std::string_view                            name
) noexcept -> std::optional<std::reference_wrapper<const shell::command_view>>
{
    // Iterate over command list and check if any command matches the typed program name.

    auto found = std::find_if(command_list.begin(), command_list.end(), [command_list, name](const auto& cmd)
    {
        return name == cmd.name();
    });

    if ( found != command_list.end() )
    {
        return std::cref(*found);
    }

    return {};
}

} // namespace

// ASCII table.

enum ascii_table_type : char
{
    k_escape               = 0x1B,
    k_backspace            = 0x08,
    k_tab                  = 0x09,
    k_delete               = 0x7F,
    k_left_square_bracket  = 0x5B,
    k_right_square_bracket = 0x5D,
    k_caret                = 0x5A,
    k_space                = 0x20,
    k_underscore           = 0x5F,
    k_minus                = 0x2D,
    k_linefeed             = 0x0A,
    k_carriage_return      = 0x0D,
    k_question_mark        = 0x3F,
    k_exclamation_mark     = 0x21,
    k_dot                  = 0x2E
};

class command_shell
{
    enum prompt_status_type : std::size_t
    {
        k_none                          = 0,
        k_status_input_line_dirty       = 1,
        k_status_input_line_append_char = 2,
        k_status_input_line_new_line    = 3,
        k_status_input_line_erase_char  = 4,
        k_status_input_line_ready       = 5
    };

    static constexpr auto k_default_input_key_buffer_size = std::size_t{64};

    public:

    explicit command_shell(std::span<const shell::command_view> command_list)
        : m_command_list{command_list}
        {}

    ~command_shell() = default;

    command_shell(const command_shell&) = delete;
    command_shell(command_shell&&)      = delete;

    const command_shell& operator=(const command_shell&) = delete;
    const command_shell& operator=(command_shell&&)      = delete;

    void list_matching_commands()
    {
        std::int32_t match{};

        for ( auto& cmd : m_command_list )
        {
            // Print command usage if it matches the input key buffer content or print
            // all commands if buffer is empty.

            if ( m_input_buffer.empty() || strstr(cmd.name().data(), m_input_buffer.data()) != nullptr )
            {
                match++;

                // Start with a new line.

                m_prompt.new_line();

                // Print command.

                m_prompt.put(cmd.name());

                constexpr auto k_command_separator = std::string_view{": "};
                constexpr auto k_command_padding   = std::uint32_t{28};

                // Print padding.

                const std::size_t len = cmd.name().size() > k_command_padding ? 0 : k_command_padding - cmd.name().size() ;
                for ( std::size_t n{}; n != len; ++n )
                {
                    m_prompt.put(k_space);
                }

                // Print command usage.

                auto description = cmd.description();
                if ( description.empty() )
                {
                    continue;
                }

                m_prompt.put(k_command_separator);
                m_prompt.put(description);
            }
        }

        // Invalidate input line if printing were done.

        if ( match )
        {
            m_prompt.new_line();
            m_input_status.set(prompt_status_type::k_status_input_line_dirty);
        }
    }

    void make_command_arguments()
    {
        if ( m_input_buffer.empty() )
        {
            return;
        }

        // FIXME: Clear previous command.

        std::fill_n(m_arg_line.data(), m_arg_line.size(), '\0');

        // Copy input buffer content.

        std::copy_n(m_input_buffer.data(), m_input_buffer.size(), std::begin(m_arg_line));

        // Null terminate argument buffer before any string operations.

        m_arg_line[m_input_buffer.size()] = '\0';

        // Split input buffer to null-separated sentences.

        if ( auto count = shell::split_arguments(m_arg_line, m_argv_buffer); count > 0 )
        {
            // Generate program arguments.

            m_args = shell::arguments{std::span{m_argv_buffer.begin(), count}};
        }

        // Clear previous command arguments.

        else if ( m_args.empty() == false )
        {
            m_args.clear();
        }
    }

    void try_auto_complete_program_name()
    {
        const shell::command_view* candidate{};
        std::size_t                cmd_len{};
        std::size_t                match_count{};

        // Null terminate input buffer.

        m_input_buffer.push_back('\0');

        // Iterate over the command list. Auto-complete input buffer with most matching command.

        for ( auto& cmd : m_command_list )
        {
            if ( cmd.name().size() < m_input_buffer.size() )
            {
                continue;
            }

            if ( starts_with(cmd.name(), m_input_buffer) == false )
            {
                continue;
            }

            match_count += 1;

            if ( candidate == nullptr )
            {
                candidate = std::addressof(cmd);
                cmd_len   = cmd.name().size();
            }

            const auto common_length = util::find_common_length(candidate->name().data(),
                                                                candidate->name().size(),
                                                                cmd.name().data(),
                                                                cmd.name().size());

            if ( cmd_len > common_length )
            {
                candidate = std::addressof(cmd);
                cmd_len   = common_length;
            }
        }

        // Fill input buffer with a command candidate.

        if ( candidate != nullptr )
        {
            cmd_len = match_count == 1 ? candidate->name().size() : cmd_len;

            m_input_status.set(prompt_status_type::k_status_input_line_dirty);
            m_input_buffer.clear();

            std::copy_n(std::cbegin(candidate->name()), cmd_len, std::back_inserter(m_input_buffer));
        }
    }

    void try_auto_complete_program_argument(const shell::command_view& cmd)
    {
        // Check if the command implements the argument auto completion callback.

        const auto completion = cmd.autocomplete_helper();
        if ( completion.has_value() == false )
        {
            return;
        }

        // Invoke the argument auto completion callback.

        std::array<char, 128> temp{};
        temp[0] = '\0';

        std::invoke(completion.value(), m_args[0], std::span{temp});

        // Check result.

        auto n = strnlen(temp.data(), temp.size());
        if ( n < 1 || n >= temp.size() )
        {
            return;
        }

        m_input_status.set(prompt_status_type::k_status_input_line_dirty);
        m_input_buffer.clear();

        // Append the result at the the of the input buffer.

        std::copy(std::cbegin(cmd.name()), std::cend(cmd.name()), std::back_inserter(m_input_buffer));
        m_input_buffer.push_back(' ');
        std::copy_n(std::cbegin(temp), n, std::back_inserter(m_input_buffer));
    }

    void on_tab_key_event()
    {
        // Print all commands and their usage if input buffer is empty.

        if ( m_input_buffer.size() == 0 )
        {
            list_matching_commands();
            return;
        }

        make_command_arguments();

        // Attempt to auto complete the command name.

        if ( m_args.size() == 0 )
        {
            try_auto_complete_program_name();
        }

        // Attempt to auto complete the command argument.

        else if ( const auto cmd = find_program(m_command_list, m_args.program_name()); cmd.has_value() )
        {
            try_auto_complete_program_argument(cmd.value().get());
        }

        // Print all matched commands if there is more than one match.

        // if ( match_count > 1 )
        // {
        //     list_matching_commands();
        // }
    }

    void on_backspace_key_event()
    {
        if ( m_input_buffer.empty() == false )
        {
            m_input_buffer.pop_back();
            m_input_status.set(prompt_status_type::k_status_input_line_erase_char);
        }
    }

    void on_up_down_key_event(bool up)
    {
        auto data = up ? m_history.next() : m_history.prev();
        if ( data.empty() == false )
        {
            m_input_buffer.clear();

            std::copy_n(std::begin(data), data.size(), std::back_inserter(m_input_buffer));
            m_input_status.set(prompt_status_type::k_status_input_line_dirty);
        }
    }

    void on_enter_key_event()
    {
        m_input_status.set(prompt_status_type::k_status_input_line_new_line);

        // Update input line.

        refresh_inputline();

        if ( m_input_buffer.empty() )
        {
            return;
        }

        // Null terminate input buffer.

        m_input_buffer.push_back('\0');

        // Make command arguments.

        make_command_arguments();

        if ( m_args.empty() == false )
        {
            // Save input buffer content to the command history.

            m_history.push(m_input_buffer);

            // Update status to indicate that input line may contain a valid command name (and possibly additional arguments).

            m_input_status.set(prompt_status_type::k_status_input_line_ready);
        }

        // Clear input buffer.

        m_input_buffer.clear();
    }

    // refresh_inputline

    void refresh_inputline()
    {
        // Erase whole input line and echo content on the input buffer to tty.

        if ( status_test_reset(prompt_status_type::k_status_input_line_dirty, m_input_status) )
        {
            m_prompt.clear_line();
            m_prompt.Prompt();

            // Write input key buffer content.

            if ( m_input_buffer.empty() == false )
            {
                m_prompt.put(std::string_view{m_input_buffer.data(), m_input_buffer.size()});
                m_input_status.reset(prompt_status_type::k_status_input_line_append_char);
            }
        }

        // Append one char to end of the input input line.

        if ( status_test_reset(prompt_status_type::k_status_input_line_append_char, m_input_status)  )
        {
            m_prompt.put(m_input_buffer.back());
        }

        // Remove char from end of the input line.

        if ( status_test_reset(prompt_status_type::k_status_input_line_erase_char, m_input_status) )
        {
            m_prompt.replace(std::string_view{m_input_buffer.data(), m_input_buffer.size()});
        }

        // Switch to a new line.

        if ( status_test_reset(prompt_status_type::k_status_input_line_new_line, m_input_status) )
        {
            m_prompt.new_line();
            m_prompt.Prompt();
        }
    }

    void handle_key(shell::key_type key)
    {
        std::visit([this] (auto c)
        {
            if constexpr ( std::is_same_v<shell::special_key_type, decltype(c)> )
            {
                if ( c == shell::special_key_type::k_arrow_up )
                {
                    on_up_down_key_event(true);
                }

                else if ( c == shell::special_key_type::k_arrow_down )
                {
                    on_up_down_key_event(false);
                }

                else if ( c == shell::special_key_type::k_carriage_return || c == shell::special_key_type::k_new_line )
                {
                    on_enter_key_event();
                }

                else if ( c == shell::special_key_type::k_backspace || c == shell::special_key_type::k_delete )
                {
                    on_backspace_key_event();
                }

                else if ( c == shell::special_key_type::k_tab )
                {
                    on_tab_key_event();
                }
            }

            if constexpr ( std::is_same_v<char, decltype(c)> )
            {
                m_input_buffer.push_back(c);
                m_input_status.set(prompt_status_type::k_status_input_line_append_char);
            }

        }, key);
    }

    public:

    void process_input(shell::key_type c)
    {
        // Append incomings byte to the input buffer.

        handle_key(c);

        // Update input line.

        refresh_inputline();

        // Return if the input line is incomplete.

        if ( status_test_reset(prompt_status_type::k_status_input_line_ready, m_input_status) == false )
        {
            return;
        }

        // Return if no command name.

        if ( m_args.empty() )
        {
            return;
        }

        // Iterate over command list and check if any command matches the typed program name.

        if ( auto cmd = find_program(m_command_list, m_args.program_name()); cmd.has_value() )
        {
            // Run command.

            auto os = log::formatting_stream<shell::input_line>{m_prompt};

            std::invoke(cmd.value(), m_args, os);
        }
    }

    void run()
    {
        using namespace sys::dev;

        m_input_buffer.reserve(k_default_input_key_buffer_size);

        while ( true )
        {
            process_input(m_prompt.get());
        }
    }

    private:

    const std::span<const shell::command_view> m_command_list;
    std::bitset<6>                             m_input_status{};
    platform::pmr::vector<char>                m_input_buffer{platform::memory::default_resource()};

    std::array<char, k_default_input_key_buffer_size> m_arg_line{};
    std::array<std::string_view, 5>                   m_argv_buffer{};

    shell::input_line                                m_prompt{};
    shell::history<k_default_input_key_buffer_size>  m_history{};
    shell::arguments                                 m_args{};
};

} // sys::shell
