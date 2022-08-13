#pragma once

namespace sys::shell {

template <std::size_t k_max_line>
struct history
{
    static constexpr std::size_t k_max_count = 5;

    template <typename T>
    auto push(const T& container) noexcept
    {
        const auto count = container.back() == '\0' ? std::strlen(container.data()) + 1 : container.size();
        if ( count >= k_max_line )
        {
            return false;
        }

        const auto save_index = [this]()
        {
            if ( m_count++; m_count < k_max_count )
            {
                return m_count;
            }

            return m_count % k_max_count;
        }();

        std::copy_n(std::begin(container), count, std::begin(m_command_history[save_index]));

        return true;
    }

    auto next() noexcept -> std::string_view
    {
        if ( m_count < 1 )
        {
            return {};
        }

        m_position = m_position + 1;
        if ( m_position == k_max_count || m_position == m_count )
        {
            m_position = 0;
        }

        auto data = m_command_history[m_position].data();

        return std::string_view{data, std::strlen(data)};
    }

    auto prev() noexcept -> std::string_view
    {
        if ( m_count < 1 )
        {
            return {};
        }

        if ( m_position == 0 )
        {
            m_position = std::min(m_count, k_max_count);
        }

        m_position--;

        auto data = m_command_history[m_position].data();

        return std::string_view{data, std::strlen(data)};
    }

    private:

    std::size_t                  m_position{};
    std::size_t                  m_count{};
    std::array<char, k_max_line> m_command_history[k_max_count]{};
};

} // namespace sys::shell
