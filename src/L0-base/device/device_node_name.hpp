#pragma once

namespace sys {

struct device_node
{
    using hash_type    = decltype(util::hash_fnv1a(""));
    using address_type = std::uint32_t;

    constexpr device_node() = default;

    consteval device_node(const char* s) noexcept
    {
        std::size_t i{};

        while ( s[i] != '\0' && s[i] != '@' )
        {
            i++;
        }

        m_node_name = util::hash_fnv1a(s, i);

        if ( s[i] != '@' )
        {
            return;
        }

        s = std::addressof(s[i + 1]);
        i = 0;

        while ( s[i] != '\0' && s[i] != ',' )
        {
            m_unit_address = s[i] - '0' + m_unit_address * 10;
            i++;
        }

        if ( s[i] != '\0' )
        {
            s = std::addressof(s[i + 1]);
            i = 0;

            while ( s[i] != '\0' )
            {
                m_unit_sub_address = s[i] - '0' + m_unit_sub_address * 10;
                i++;
            }
        }
    }

    [[nodiscard]] constexpr auto operator==(const device_node& other) const noexcept -> bool
    {
        return m_node_name == other.m_node_name && m_unit_address == other.m_unit_address;
    }

    [[nodiscard]] constexpr auto operator!=(const device_node& other) const noexcept -> bool
    {
        return not (*this == other);
    }

    hash_type    m_node_name{};
    address_type m_unit_address{};
    address_type m_unit_sub_address{};
};

template <device_node kNode1, device_node kNode2>
struct instance_of_node
{
    static constexpr bool value = kNode1.m_node_name == kNode2.m_node_name;
};

template <device_node k_node>
struct unit_address_of_node
{
    static constexpr auto value = k_node.m_unit_address;
};

template <device_node k_node>
struct unit_sub_address_of_node
{
    static constexpr auto value = k_node.m_unit_sub_address;
};

template <device_node k_node>
struct register_address_of_node;

template <device_node kNode1, device_node kNode2>
struct is_same_node
{
    static constexpr bool value = []()
    {
        if ( not instance_of_node<kNode1, kNode2>::value )
            return false;

        return unit_sub_address_of_node<kNode1>::value == unit_sub_address_of_node<kNode2>::value;
    }();
};

static_assert(device_node{"/dev/tty2"}  == device_node{"/dev/tty2"}, "");
static_assert(device_node{"/dev/tty@2"} != device_node{"/dev/tty2"}, "");
static_assert(device_node{"/dev/tty@2"} != device_node{"/dev/tty@3"}, "");
static_assert(device_node{"/dev/tty@3"} == device_node{"/dev/tty@3"}, "");
static_assert(device_node{"/dev/tty@3,0"} == device_node{"/dev/tty@3"}, "");
static_assert(unit_address_of_node<"/dev/tty@2">::value == 2, "");
static_assert(unit_address_of_node<"/dev/tty@124">::value == 124, "");
static_assert(unit_sub_address_of_node<"/dev/tty@0,1">::value == 1, "");
static_assert(unit_sub_address_of_node<"/dev/tty@0,12">::value == 12, "");
static_assert(unit_sub_address_of_node<"/dev/tty@0,0">::value == 0, "");
static_assert(unit_address_of_node<"/dev/tty@0,1">::value == 0, "");

static_assert(instance_of_node<"/dev/tty@2", "/dev/tty">::value == std::true_type::value, "");
static_assert(instance_of_node<"/dev/tty@2", "/dev/aaa">::value == std::false_type::value, "");

} // namespace sys
