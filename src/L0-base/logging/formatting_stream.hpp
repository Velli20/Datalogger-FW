#pragma once

namespace sys::log {

template <typename... Args>
struct fmt
{
    explicit constexpr fmt(
        const char* format,
        Args...     args
    ) noexcept
    {
        const int result = std::snprintf(buffer, sizeof(buffer), format, args...);
        length           = result == -1 ? 0u : static_cast<std::size_t>(result);
    }

    constexpr ~fmt()                               noexcept = default;
    constexpr  fmt(fmt<Args...> const&)            noexcept = default;
    constexpr  fmt(fmt<Args...>&&)                 noexcept = default;

    constexpr fmt<Args...>& operator=(fmt<Args...> const&)  noexcept = default;
    constexpr fmt<Args...>& operator=(fmt<Args...>&&)       noexcept = default;

    char        buffer[128];
    std::size_t length;
};

enum class severity : std::size_t
{
    k_debug,
    k_info,
    k_warning,
    k_error,
};

static constexpr struct endl {} endl{};

template <class T>
struct formatting_stream : public io::stream<T>
{
    private:

    log::format_flag m_format_bits{log::format_flag::kNone};

    public:

    using io::stream<T>::out;

    explicit constexpr formatting_stream(T& s) noexcept
        : io::stream<T>{std::forward<decltype(s)>(s)}
        {}

    explicit constexpr formatting_stream(T&& s) noexcept
        : io::stream<T>{std::forward<decltype(s)>(s)}
        {}

    [[nodiscard]] constexpr auto IsFormatShowBase() const
    {
        return has_any_flag(m_format_bits, log::format_flag::kShowBase);
    }

    [[nodiscard]] constexpr auto IsFormatUpperCase() const
    {
        return has_any_flag(m_format_bits, log::format_flag::kUpperCase);
    }

    [[nodiscard]] constexpr auto GetIntegralFormat() const
    {
        constexpr log::format_flag k_formats[] =
        {
            log::format_flag::k_hex_format,
            log::format_flag::k_bin_format,
            log::format_flag::k_decimal_format
        };

        auto f = std::find_if(std::begin(k_formats), std::end(k_formats), [this](auto n) { return has_any_flag(m_format_bits, n); });
        if ( f != std::end(k_formats) )
        {
            return *f;
        }

        return log::format_flag::k_decimal_format;
    }

    [[maybe_unused]] constexpr auto operator<<(log::format_flag arg) noexcept -> log::formatting_stream<T>&
    {
        if ( arg >= log::format_flag::k_hex_format && arg <= log::format_flag::k_decimal_format )
        {
            m_format_bits &= ~(log::format_flag::k_hex_format);
            m_format_bits &= ~(log::format_flag::k_bin_format);
            m_format_bits &= ~(log::format_flag::k_decimal_format);
            m_format_bits |=  (arg);
        }

        else if ( has_any_flag(arg, log::format_flag::kClear) == true )
        {
            m_format_bits &= ~arg;
        }

        else
        {
            m_format_bits |= arg;
        }

        return *this;
    }

    template <typename... Args>
    [[maybe_unused]] constexpr auto operator<<(const log::fmt<Args...>&& arg) noexcept -> log::formatting_stream<T>&
    {
        out(std::string_view{arg.buffer, arg.length});

        return *this;
    }

    [[maybe_unused]] constexpr auto operator<<(struct endl) noexcept -> log::formatting_stream<T>&
    {
        out('\r');
        out('\n');

        return *this;
    }

    [[maybe_unused]] constexpr auto operator<<(const char* data) noexcept -> log::formatting_stream<T>&
    {
        if ( data == nullptr )
            return *this;

        if ( auto len = std::strlen(data); len > 1 )
        {
            out(std::string_view{data, len});
        }

        return *this;
    }

    [[maybe_unused]] constexpr auto operator<<(char arg) noexcept -> log::formatting_stream<T>&
    {
        out(arg);

        return *this;
    }

    [[maybe_unused]] constexpr auto operator<<(std::string_view s) noexcept -> log::formatting_stream<T>&
    {
        out(s);

        return *this;
    }

    constexpr auto operator<<(const std::error_code& arg) noexcept -> log::formatting_stream<T>&
    {
        out('[');

        {
            out(arg.category().name());
        }

        out("] ");

        if ( arg.message().empty() == false )
        {
            out(std::string_view{arg.message().data(), arg.message().size()});
        }

        else
        {
            out(std::string_view{"(No error message available)"});
        }

        return *this;
    }
};

} // namespace sys::log
