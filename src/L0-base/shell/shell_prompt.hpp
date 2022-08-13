#pragma once

namespace sys::shell {

enum class special_key_type
{
    k_none,
    k_arrow_up,
    k_arrow_down,
    k_arrow_right,
    k_arrow_left,
    k_end,
    k_home,
    k_backspace,
    k_delete,
    k_carriage_return,
    k_new_line,
    k_tab,
};

using key_type = std::variant<
    char,
    shell::special_key_type
>;

struct input_line final : public io::output_sink_interface
{
    private:

    enum /* class */ ansi_type : std::uint8_t
    {
        k_null                      = 0x00,
        k_start_of_header           = 0x01,
        k_start_of_text             = 0x02,
        k_end_of_text               = 0x03,
        k_end_of_transmission       = 0x04,
        k_enquiry                   = 0x05,
        k_acknowledgment            = 0x06,
        k_bell                      = 0x07,
        k_backspace                 = 0x08,
        k_horizontal_tab            = 0x09,
        k_line_feed                 = 0x0A,
        k_vertical_tab              = 0x0B,
        k_form_feed                 = 0x0C,
        k_carriage_return           = 0x0D,
        k_shift_out                 = 0x0E,
        k_shift_in                  = 0x0F,
        k_data_link_escape          = 0x10,
        k_xon_device_control1       = 0x11,
        k_device_control2           = 0x12,
        k_xoff_device_control3      = 0x13,
        k_device_control4           = 0x14,
        k_negative_ack              = 0x15,
        k_synchronous_idle          = 0x16,
        k_end_of_transmission_block = 0x17,
        k_cancel                    = 0x18,
        k_end_of_medium             = 0x19,
        k_substitute                = 0x1A,
        k_escape                    = 0x1B,
        k_file_separator            = 0x1C,
        k_group_separator           = 0x1D,
        k_record_separator          = 0x1E,
        k_unit_separator            = 0x1F,
        k_delete                    = 0x7F
    };

    [[nodiscard]] char read_one()
    {
        using namespace sys::dev;

        std::byte data{};

        do
        {
            m_stream >> data;
            if ( data == std::byte {} )
            {
                continue;
            }

            return std::to_integer<char>(data);
        }
        while ( true );
    }

    public:

    constexpr auto operator<<(std::span<const std::byte> data) noexcept -> shell::input_line& override
    {
        if ( std::is_constant_evaluated() )
        {
            return *this;
        }

        // Clear input_line and switch to new line if stream output stream is used.

        if ( m_log_stream_used != true )
        {
            new_line();
        }

        m_log_stream_used = true;

        m_stream << data;

        return *this;
    }

    constexpr auto operator<<(std::byte b) noexcept -> shell::input_line& override
    {
        if ( std::is_constant_evaluated() )
        {
            return *this;
        }

        // Clear input_line and switch to new line if stream output stream is used.

        if ( m_log_stream_used != true )
        {
            new_line();
        }

        m_log_stream_used = true;

        m_stream << b;

        return *this;
    }

    void clear_line() noexcept
    {
        constexpr auto k_clear = std::string_view{"\r\x1b[0K"};

        put(k_clear);
        m_cursor_position = 0;
    }

    void new_line() noexcept
    {
        constexpr auto k_new_line = std::string_view{"\r\n"};

        put(k_new_line);
        m_cursor_position = 0;
    }

    void Prompt()
    {
        constexpr auto k_prompt = std::string_view{"[sys] "};

        put(k_prompt);
        m_cursor_position = 0;
    }

    void put(char c) noexcept
    {
        if ( m_log_stream_used )
        {
            m_log_stream_used = false;
            new_line();
        }

        const auto out = std::span(std::addressof(c), 1);

        m_stream << std::as_bytes(out);

        if ( c != ansi_type::k_line_feed && c != ansi_type::k_carriage_return )
        {
            m_cursor_position += 1;
        }
    }

    void put(const std::string_view s) noexcept
    {
        if ( s.empty() )
            return;

        for ( auto c : s ) { put(c); }
    }

    void move_to(std::size_t pos) noexcept
    {
        if ( pos >= sizeof("[sys] ") - 1 )
        {
            pos = sizeof("[sys] ") - 1;
        }

        constexpr auto k_move_to_format = std::string_view{"\x1b[%dD"};

        char temp[32];
        std::snprintf(temp, sizeof(temp), k_move_to_format.data(), pos);
        temp[31] = 0;

        put(std::string_view{temp, std::strlen(temp)});

        m_cursor_position = pos;
    }

    void replace(const std::string_view s) noexcept
    {
        // Move cursor position to left by n-number of charachters.

        if ( m_cursor_position > s.size() )
        {
            move_to(m_cursor_position - s.size());
        }

        constexpr auto k_clearEnd = std::string_view{"\x1b[0K"};

        put(k_clearEnd);
        m_cursor_position = s.size();
    }

    [[nodiscard]] auto get() noexcept -> shell::key_type
    {
        struct key_fsm_type
        {
            char                          expecting;
            std::span<const key_fsm_type> branch;
            shell::special_key_type       key;

            [[nodiscard]] constexpr auto operator==(char other) const
            {
                return expecting == other;
            }
        };

        static constexpr key_fsm_type k_control_sequence_table[] =
        {
            { 'A', {}, shell::special_key_type::k_arrow_up    },
            { 'B', {}, shell::special_key_type::k_arrow_down  },
            { 'C', {}, shell::special_key_type::k_arrow_right },
            { 'D', {}, shell::special_key_type::k_arrow_left  },
            { 'F', {}, shell::special_key_type::k_end         },
            { 'H', {}, shell::special_key_type::k_home        },
        };

        static constexpr key_fsm_type k_key_table_a[] =
        {
            { '[', std::span{k_control_sequence_table}, {} },
        };

        static constexpr key_fsm_type k_key_table_b[] =
        {
            { ansi_type::k_carriage_return, {},                       shell::special_key_type::k_carriage_return },
            { ansi_type::k_line_feed,       {},                       shell::special_key_type::k_new_line        },
            { ansi_type::k_backspace,       {},                       shell::special_key_type::k_backspace       },
            { ansi_type::k_horizontal_tab,  {},                       shell::special_key_type::k_tab             },
            { ansi_type::k_delete,          {},                       shell::special_key_type::k_delete          },
            { ansi_type::k_escape,          std::span{k_key_table_a}, {}                                         },
        };

        std::span<const key_fsm_type> current_branch = std::span{k_key_table_b};

        do
        {
            auto key = read_one();
            auto it  = std::find_if(current_branch.begin(), current_branch.end(), [key](auto n) { return n.expecting == key; });
            if ( it != current_branch.end() )
            {
                if ( it->branch.empty() )
                {
                    return it->key;
                }

                current_branch = it->branch;
                continue;
            }

            return key;
        }
        while ( true );

        return {};
    }

    private:

    bool                  m_log_stream_used{};
    std::size_t           m_cursor_position{};
    io::stdio_stream_sink m_stream{};
};

} // namespace sys::shell
