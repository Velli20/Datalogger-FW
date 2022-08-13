#pragma once

namespace sys {

enum class directory_entry_type
{
    k_directory,
    k_regular_file,
    k_socket,
    k_block_device,
    k_symbolic_link,
    k_sentinel
};

struct directory_entry
{
    directory_entry(
        const std::string    name,
        directory_entry_type type
    )
    : m_name{std::move(name)}
    , m_type{type}
    {}

    directory_entry()
    : m_name{}
    , m_type{directory_entry_type::k_sentinel}
    {}

    public:

    auto type() const  { return m_type; }
    auto name() const& { return m_name; }

    private:

    std::string                m_name;
    const directory_entry_type m_type;
};

struct directory
{
    constexpr directory() noexcept {}

    constexpr directory(const directory&) = delete;
    constexpr directory(directory&&)      = delete;

    [[maybe_unused]]
    constexpr auto operator=(directory&&) noexcept -> directory & = delete;

    [[maybe_unused]]
    constexpr auto operator=(const directory&) noexcept -> directory & = delete;

    /// Destructor of this base class calls the destructors of derived classes.

    constexpr virtual ~directory() = default;

    constexpr virtual bool             rewind()                          noexcept = 0;
    constexpr virtual bool             close()                           noexcept = 0;
    constexpr virtual bool             open(const std::filesystem::path) noexcept = 0;
    constexpr virtual sys::error_code  error_code()                      noexcept = 0;
    constexpr virtual directory_entry  read()                            noexcept = 0;
};

} // namespace sys
