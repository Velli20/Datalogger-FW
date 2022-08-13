#pragma once

namespace sys::hal::jedec {

enum class sector_type
{
    k_sector1,
    k_sector2,
    k_sector3,
    k_sector4
};

enum class mode_type : std::size_t
{
    k114_fast_read = 0,
    k144_fast_read,
    k122_fast_read,
    k112_fast_read,
    k444_fast_read,
    k222_fast_read
};

struct [[gnu::packed]] parameter_table
{
    constexpr parameter_table() = default;

    struct [[gnu::packed]] sector
    {
        // This field will be 0x00 if this sector type does not exist.
        // NOTE: This field specifies ‘N’ and is used to calculate sector/block size = 2^N bytes.

        std::size_t size : 8 {};

        // Opcode used to erase the number of bytes specified by Sector Type N Size (bits 23-16).

        std::uint32_t erase_op_code : 8 {};
    };

    static_assert(sizeof(sector) == 2);

    struct [[gnu::packed]] mode_base
    {
        // <Mode> Opcode.

        std::uint8_t op_code{};

        // <Mode> Number of Mode Bits.

        std::uint8_t mode_bits : 3 {};

        // <Mode> Number of Wait states (dummy clocks) needed before valid output.

        std::uint8_t wait_states : 5 {};
    };

    static_assert(sizeof(mode_base) == 2);

    template <jedec::mode_type k_mode, typename T=void>
    struct mode { };

    template <jedec::mode_type k_mode>
    struct [[gnu::packed]] mode <k_mode, typename std::enable_if<k_mode == jedec::mode_type::k114_fast_read ||
                                                                k_mode == jedec::mode_type::k144_fast_read ||
                                                                k_mode == jedec::mode_type::k122_fast_read ||
                                                                k_mode == jedec::mode_type::k112_fast_read>::type>
    : public mode_base {};

    template <jedec::mode_type k_mode>
    struct [[gnu::packed]] mode <k_mode, typename std::enable_if<k_mode == jedec::mode_type::k444_fast_read ||
                                                                k_mode == jedec::mode_type::k222_fast_read>::type>
    : public mode_base
    {
        std::uint16_t reserved{};
    };

    static_assert(sizeof(mode<jedec::mode_type::k114_fast_read>) == 2);
    static_assert(sizeof(mode<jedec::mode_type::k222_fast_read>) == 4);

    public:

    // Block/Sector Erase Size.

    [[nodiscard]] auto erase_size() const -> std::optional<std::uint32_t>
    {
        const std::bitset<32> bits{m_data[0]};

        /*
            Bit 00: Reserved
            Bit 01: 4 kilobyte Erase
            Bit 10: Reserved
            Bit 11: Use this setting only if the 4 kilobyte erase is unavailable
        */

        if ( bits.test(0) == true && bits.test(1) == false )
        {
            return 4096;
        }

        return {};
    }

    [[nodiscard]] auto write_granuality() const -> std::size_t
    {
        const std::bitset<32> bits{m_data[0]};

        /*
            Bit 0: 1 Byte – Use this setting for single byte programmable devices or buffer programmable
                   devices when the buffer is less than 64 bytes (32 Words).
            Bit 1: Use this setting for buffer programmable devices when the buffer size is
                   64 bytes (32 Words) or larger.
        */

        return bits.test(2) ? 64 : 1;
    }

    [[nodiscard]] auto is_non_volatile() const
    {
        const std::bitset<32> bits{m_data[0]};

        /*
            Bit 0: Target flash has nonvolatile status bit and does not require
                   status register to be written on every power on to allow writes and erases.
            Bit 1: Target flash requires a 0x00 to be written to the status register
                   in order to allow writes and erases.
        */

        return bits.test(3) == 0 ? true : false;
    }

    // Write Enable Opcode Select for Writing to Volatile Status mmio::register_type

    [[nodiscard]] auto write_voltile_status_register_enable_op() const -> std::optional<std::uint8_t>
    {
        const std::bitset<32> bits{m_data[0]};

        /*
            Bit 0: 0x50 is the Opcode to enable a status register write when bit 3 is set to 1.
            Bit 1: 0x06 is the Opcode to enable a status register write when bit 3 is set to 1.
        */

        if ( bits.test(3) == true )
        {
            return bits.test(4) ? 0x50 : 0x06;
        }

        return {};
    }

    [[nodiscard]] auto density() const -> std::size_t
    {
        return static_cast<std::size_t>(m_data[1]);
    }

    [[nodiscard]] auto sector(sector_type s) const -> std::optional<std::tuple<jedec::cfi_command, std::size_t>>
    {
        const auto index = [=]() -> std::size_t
        {
            if ( s == sector_type::k_sector1 ) { return 1; }
            if ( s == sector_type::k_sector2 ) { return 0; }
            if ( s == sector_type::k_sector3 ) { return 2; }
            /* if ( s == sector_type::kSector4 ) */ { return 3; }
        }();

        if ( m_sector[index].size != 0u )
        {
            return { std::make_tuple(static_cast<jedec::cfi_command>(m_sector[index].erase_op_code), std::pow(2, m_sector[index].size)) };

        }

        return {};
    }

    private:

    std::uint32_t m_data[7]{};
    struct sector m_sector[4]{};
};

} // namespace sys::hal::jedec
