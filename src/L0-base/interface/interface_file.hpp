#pragma once

namespace sys {

enum class file_seek_origin_type
{
    k_set,
    k_current,
    k_end,
};

enum class file_mode_type
{
    k_read,
    k_read_write,
    k_read_write_create_always,
    k_append,
};

struct file
{
    constexpr file() noexcept {}

    constexpr file(const file&) = delete;
    constexpr file(file&&)      = delete;

    [[maybe_unused]] constexpr auto operator=(file&&)      noexcept -> file& = delete;
    [[maybe_unused]] constexpr auto operator=(const file&) noexcept -> file& = delete;

    virtual ~file() = default;

    virtual std::size_t     write(const std::span<const std::byte>)       noexcept = 0;
    virtual std::size_t     read (std::span<std::byte>)                   noexcept = 0;
    virtual std::size_t     size()                                        noexcept = 0;
    virtual std::size_t     tell()                                        noexcept = 0;
    virtual std::size_t     seek(std::size_t, file_seek_origin_type)           noexcept = 0;
    virtual bool            open(const std::filesystem::path&, file_mode_type) noexcept = 0;
    virtual bool            close()                                       noexcept = 0;
    virtual bool            is_open()                                     noexcept = 0;
    virtual sys::error_code error_code()                                  noexcept = 0;
};

} // namespace sys
