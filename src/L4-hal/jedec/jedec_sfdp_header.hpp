#pragma once

namespace sys::hal::jedec {

struct [[gnu::packed]] sfdp_header
{
    enum class manufacturer_id_type : std::uint8_t
    {
        k_jedec      = 0x00,
        k_gigadevice = 0xC8
    };

    std::uint8_t h_signature[4];
    std::uint8_t h_version_minor;
    std::uint8_t h_version_major;
    std::uint8_t h_number_of_parameters_headers;
    std::uint8_t h_unused;

    // Paramater header data.

    std::uint8_t  p_id;
    std::uint8_t  p_version_minor;
    std::uint8_t  p_version_major;
    std::uint8_t  p_table_length;
    std::uint32_t p_table_pointer : 24;
    std::uint8_t  p_unused;

    [[nodiscard]] auto is_valid() const -> bool
    {
        constexpr std::array<std::uint8_t, 4> k_expected {{ 'S', 'F', 'D', 'P'}};

        return std::equal(std::cbegin(k_expected), std::cend(k_expected), std::cbegin(h_signature));
    }

    // Returns SFDP Major Revision Number.

    [[nodiscard]] auto version_major() const
    {
        return static_cast<std::uint32_t>(h_version_major);
    }

    // Returns SFDP Minor Revision Number.

    [[nodiscard]] auto version_minor() const
    {
        return static_cast<std::uint32_t>(h_version_minor);
    }

    // Returns Number of Parameters Headers.

    [[nodiscard]] auto param_header_count() const
    {
        return static_cast<std::uint32_t>(h_number_of_parameters_headers);
    }

    [[nodiscard]] auto param_version_major() const -> std::uint8_t
    {
        return p_version_major;
    }

    [[nodiscard]] auto param_version_minor() const -> std::uint8_t
    {
        return p_version_minor;
    }

    // Returns amount of DWORDs in the Parameter table.

    [[nodiscard]] auto param_table_length() const -> std::uint8_t
    {
        return p_table_length;
    }

    // Parameter Table Pointer (PTP). First address of JEDEC Flash Parameter table
    // or First address of the manufacturer Flash Parameter table.

    [[nodiscard]] auto param_table_pointer() const -> std::uint32_t
    {
        return p_table_pointer;
    }

    // Manufacturer ID number.

    [[nodiscard]] auto manufacturer_id() const -> manufacturer_id_type
    {
        return static_cast<manufacturer_id_type>(p_id);
    }
};

static_assert(sizeof(jedec::sfdp_header) == 16);

} // namespace sys::hal::jedec
