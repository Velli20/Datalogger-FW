#pragma once

namespace sys::mmio {

template <class T>
constexpr T& place_at(std::uintptr_t p)
{
    return *(std::bit_cast<T*>(p));
}

template <typename T>
struct register_traits_base
{
    using type                  = T;
    static constexpr auto width = std::numeric_limits<T>::digits;
};

template <field_size_type k_field_size>
struct register_traits {};

template <>
struct register_traits <field_size_type::u32> : public register_traits_base<std::uint32_t> {};

template <>
struct register_traits <field_size_type::u16> : public register_traits_base<std::uint16_t> {};

template <>
struct register_traits <field_size_type::u8> : public register_traits_base<std::uint8_t> {};

template <field_size_type k_field_size, auto k_width = register_traits<k_field_size>::width, std::uint32_t k_offset = 0ul, typename DataValueType = typename register_traits<k_field_size>::type>
struct register_mask_type
{
    using T = typename register_traits<k_field_size>::type;
    using data_value_type = DataValueType;

    static_assert(std::numeric_limits<T>::is_integer == true);
    static_assert(std::is_unsigned<T>::value);
    static_assert(std::numeric_limits<T>::digits >= k_offset + k_width, "Error: Bit overflow");

    static constexpr auto offset = k_offset;
    static constexpr T    value  =
        k_width == std::numeric_limits<T>::digits ?
            std::numeric_limits<T>::max() :
            (((1ul << k_width) - 1u) << k_offset);
};

namespace detail {

template <typename WriteValueType, typename RegisterType, typename Mask> requires(concepts::is_writable_data_value<WriteValueType, Mask>)
constexpr auto reqister_write(WriteValueType value, RegisterType reg) noexcept
{
    return ((reg & ~Mask::value) | ((value << Mask::offset) & Mask::value));
}

template <typename WriteValueType, typename RegisterType, typename Mask> requires(concepts::is_writable_enum_value<WriteValueType, Mask>)
constexpr auto reqister_write(WriteValueType value, RegisterType reg) noexcept
{
    static_assert(std::is_convertible<WriteValueType, register_data_type<Mask>>::value);

    using Type = typename std::underlying_type<WriteValueType>::type;

    return ((reg & ~Mask::value) | ((static_cast<Type>(value) << Mask::offset) & Mask::value));
}

template <typename RegisterType, typename Mask> requires (std::negation_v<std::is_enum<register_data_type<Mask>>>)
constexpr auto reqister_read(const RegisterType reg)
{
    return ((reg & (Mask::value)) >> Mask::offset);
}

template <typename RegisterType, typename Mask> requires (std::is_enum_v<register_data_type<Mask>>)
constexpr auto reqister_read(const RegisterType reg)
{
    return static_cast<register_data_type<Mask>>(((reg & (Mask::value)) >> Mask::offset));
}

template <typename RegisterType, typename Mask>
constexpr auto reqister_set(RegisterType m_register) noexcept
{
    return m_register | (Mask::value);
}

template <typename RegisterType, typename Mask>
constexpr auto reqister_clear(RegisterType m_register) noexcept
{
    return m_register & ~(Mask::value);
}

template <typename RegisterType, typename Mask>
constexpr auto reqister_toggle(RegisterType m_register) noexcept
{
    return m_register ^ Mask::value;
}

} // namespace detail

template <field_size_type k_field_size, std::uint32_t k_field_width, std::uint32_t k_bit_offset, typename DataValueType = std::uint32_t>
struct bitmask_type final
{
    using value_type = typename register_traits<k_field_size>::type;
    using mask_type  = register_mask_type<k_field_size, k_field_width, k_bit_offset, DataValueType>;

    template <typename WriteValueType>
    constexpr auto Write(WriteValueType value) noexcept
    {
        m_register = detail::reqister_write<WriteValueType, value_type, mask_type>(value, m_register);
    }

    constexpr auto Set() noexcept
    {
        m_register = detail::reqister_set<value_type, mask_type>(m_register);
    }

    constexpr auto Read() const noexcept
    {
        return detail::reqister_read<value_type, mask_type>(m_register);
    }

    constexpr auto Clear() noexcept
    {
        m_register = detail::reqister_clear<value_type, mask_type>(m_register);
    }

    constexpr auto Toggle() noexcept
    {
        m_register = detail::reqister_toggle<value_type, mask_type>(m_register);
    }

    private:

    volatile value_type m_register;
};

template <field_size_type k_field_size>
struct register_type
{
    using value_type = typename register_traits<k_field_size>::type;

    template <std::uint32_t k_field_width, std::uint32_t kFieldOffset, typename DataValueType>
    struct register_reference_type
    {
        using Mask = register_mask_type<k_field_size, k_field_width, kFieldOffset, DataValueType>;

        template <typename WriteValueType>
        constexpr auto Write(WriteValueType value) noexcept
        {
            m_register.Write(detail::reqister_write<WriteValueType, value_type, Mask>(value, m_register.Read()));
        }

        constexpr auto Set() noexcept
        {
            m_register.Write(detail::reqister_set<value_type, Mask>(m_register.Read()));
        }

        constexpr DataValueType Read() const noexcept
        {
            return detail::reqister_read<value_type, Mask>(m_register.Read());
        }

        constexpr auto Clear() noexcept
        {
            m_register.Write(detail::reqister_clear<value_type, Mask>(m_register.Read()));
        }

        constexpr auto Toggle() noexcept
        {
            m_register.Write(detail::reqister_toggle<value_type, Mask>(m_register.Read()));
        }

        mmio::register_type<k_field_size>& m_register;
    };

    struct register_editor_type
    {
        template <typename Mask, typename WriteValueType = register_data_type<Mask>>
        constexpr auto Write(WriteValueType value) noexcept
        {
            m_value = detail::reqister_write<WriteValueType, value_type, Mask>(value, m_value);
            return *this;
        }

        template <typename Mask>
        constexpr auto Read() const noexcept -> value_type
        {
            m_value = detail::reqister_read<value_type, Mask>(m_value);
            return *this;
        }

        template <typename Mask>
        constexpr auto Set() noexcept
        {
            m_value = detail::reqister_set<value_type, Mask>(m_value);
            return *this;
        }

        template <typename Mask>
        constexpr auto Clear() noexcept
        {
            m_value = detail::reqister_clear<value_type, Mask>(m_value);
            return *this;
        }

        template <typename Mask>
        constexpr auto Toggle() noexcept
        {
            m_value = detail::reqister_toggle<value_type, Mask>(m_value);
            return *this;
        }

        constexpr auto Save() noexcept
        {
            using mask_type = register_mask_type<k_field_size>;

            m_register.Write(detail::reqister_write<value_type, value_type, mask_type>(m_value, m_register.Read()));
        }

        constexpr auto Value() noexcept
        {
            return m_value;
        }

        mmio::register_type<k_field_size>& m_register;
        value_type               m_value{m_register.Read()};
    };

    struct register_view_type
    {
        template <typename Mask>
        constexpr auto Read() const noexcept -> value_type
        {
            return detail::reqister_read<value_type, Mask>(m_value);
        }

        value_type m_value;
    };

    constexpr auto Read() const -> value_type
    {
        return m_register;
    }

    template <typename Mask>
    constexpr auto Read() const noexcept
    {
        return detail::reqister_read<value_type, Mask>(m_register);
    }

    constexpr void Write(value_type value)
    {
        m_register = value;
    }

    template <typename Mask, typename WriteValueType = register_data_type<Mask>>
    constexpr auto Write(WriteValueType value) noexcept
    {
        m_register = detail::reqister_write<WriteValueType, value_type, Mask>(value, m_register);
    }

    constexpr void Clear()
    {
        m_register = detail::reqister_clear<value_type, register_mask_type<k_field_size>>(m_register);
    }

    template <typename Mask>
    constexpr void Clear()
    {
        m_register = detail::reqister_clear<value_type, Mask>(m_register);
    }

    template <typename Mask>
    constexpr void Set()
    {
        m_register = detail::reqister_set<value_type, Mask>(m_register);
    }

    template <std::uint32_t k_field_width, std::uint32_t kFieldOffset, typename DataValueType = value_type>
    constexpr auto Field()
    {
        static_assert(k_field_width + kFieldOffset <= register_traits<k_field_size>::width);

        return register_reference_type<k_field_width, kFieldOffset, DataValueType>{*this};
    }

    constexpr auto Edit()
    {
        return register_editor_type{*this};
    }

    constexpr auto View() const
    {
        return register_view_type{Read()};
    }

    constexpr auto& operator()()
    {
        return *this;
    }

    private:

    volatile value_type m_register;
};

} // namespace sys
