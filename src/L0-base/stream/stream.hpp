#pragma once

namespace sys::io {

template <class T> requires(io::concepts::writable<T>)
struct stream
{
    private:

    T m_sink;

    public:

    explicit constexpr stream(T& s) noexcept
        : m_sink{s}
        {}

    explicit constexpr stream(T&& s) noexcept
        : m_sink{std::move(s)}
        {}

#if not defined(__apple_build_version__)
    constexpr virtual ~stream() requires (io::concepts::closable<T>)
    {
        m_sink.close();
    }
#endif

    constexpr virtual ~stream() = default;

    constexpr stream(stream const&) noexcept = default;
    constexpr stream(stream&&)      noexcept = default;

    [[maybe_unused]] constexpr auto operator=(stream const&) noexcept -> stream& = default;
    [[maybe_unused]] constexpr auto operator=(stream&&)      noexcept -> stream& = default;

    constexpr auto out(std::string_view s)
    {
        if ( std::is_constant_evaluated() )
            return;

        if ( s.empty() )
            return;

        m_sink << util::as_byte_data(s);
    }

    [[maybe_unused]] constexpr auto out(char c)
    {
        if ( std::is_constant_evaluated() == false )
        {
            m_sink << static_cast<std::byte>(c);
        }
    }
};

} // namespace sys::log
