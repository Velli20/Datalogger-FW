#pragma once

namespace sys::io {

struct standard_filestream
{
    explicit standard_filestream(const char* path)
    {
        m_handle = std::fopen(path, "a+");
        debug_assert(m_handle != nullptr);
    }

    constexpr standard_filestream(standard_filestream const&) noexcept = default;
    constexpr standard_filestream(standard_filestream&&)      noexcept = default;
    constexpr ~standard_filestream()                          noexcept = default;

    [[maybe_unused]] auto operator=(standard_filestream&&)      noexcept -> standard_filestream& = default;
    [[maybe_unused]] auto operator=(const standard_filestream&) noexcept -> standard_filestream& = default;

    [[maybe_unused]] auto operator<<(std::span<const std::byte> buffer) -> standard_filestream&
    {
        if ( buffer.empty() )
            return *this;

        debug_assert(m_handle != nullptr);

        auto n = std::fwrite(buffer.data(), sizeof(std::byte), buffer.size(), this->m_handle);
        debug_assert(n == buffer.size());

        return *this;
    }

    [[maybe_unused]] auto operator<<(std::byte b) -> standard_filestream&
    {
        debug_assert(m_handle != nullptr);

        auto n = std::fwrite(std::addressof(b), sizeof(std::byte), 1, this->m_handle);
        debug_assert(n == 1);

        return *this;
    }

    [[maybe_unused]] bool close()
    {
        std::fclose(m_handle);

        return true;
    }

    private:

    std::FILE* m_handle;
};

} // namespace sys::io

