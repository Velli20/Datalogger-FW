#pragma once

namespace sys::hal {

struct ospi_command final : public OSPI_RegularCmdTypeDef
{
    constexpr ospi_command(
        jedec::cfi_command instruction,
        std::uint32_t      data_mode    = HAL_OSPI_DATA_NONE,
        std::size_t        data_count   = 0,
        std::uint32_t      address_mode = HAL_OSPI_ADDRESS_NONE
    )
    {
        OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
        FlashId               = HAL_OSPI_FLASH_ID_1;

        InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
        InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS;
        InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;

        Address               = 0;
        AddressMode           = address_mode;
        AddressSize           = HAL_OSPI_ADDRESS_24_BITS;
        AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;

        AlternateBytes        = 0x00;
        AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
        AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
        AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;

        DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
        DummyCycles           = 0;

        DQSMode               = HAL_OSPI_DQS_DISABLE;
        SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

        DataMode              = data_mode;
        Instruction           = static_cast<std::uint32_t>(instruction);
        NbData                = static_cast<std::uint32_t>(data_count);

        // From JEDEC Standard:
        // "Following the address, eight dummy clocks (8 wait states) are required before valid data is clocked out."

        if ( instruction == jedec::cfi_command::k_jedec_sfdp )
        {
            DummyCycles = 8;
        }
    }

    operator OSPI_RegularCmdTypeDef*() noexcept
    {
        return this;
    }

    [[maybe_unused]] auto set_address(std::uint32_t address) noexcept -> decltype(auto)
    {
        this->Address = address;
        return *this;
    }

    [[maybe_unused]] auto set_address_mode(std::uint32_t address_mode) noexcept -> decltype(auto)
    {
        this->AddressMode = address_mode;
        return *this;
    }

    [[maybe_unused]] auto set_data_count(std::size_t data_count) noexcept -> decltype(auto)
    {
        this->NbData = static_cast<std::uint32_t>(data_count);
        return *this;
    }
};

} // namespace sys::hal
