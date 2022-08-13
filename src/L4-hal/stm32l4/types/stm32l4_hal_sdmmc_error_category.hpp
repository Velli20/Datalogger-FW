#pragma once

namespace sys::hal {

enum class hal_sdmmc_error : std::uint32_t
{
    no_error                 = HAL_SD_ERROR_NONE,
    command_crc_failed       = HAL_SD_ERROR_CMD_CRC_FAIL,
    data_crc_failed          = HAL_SD_ERROR_DATA_CRC_FAIL,
    command_response_timeout = HAL_SD_ERROR_CMD_RSP_TIMEOUT,
    data_response_timeout    = HAL_SD_ERROR_DATA_TIMEOUT,
    tx_underrun              = HAL_SD_ERROR_TX_UNDERRUN,
    rx_overrun               = HAL_SD_ERROR_RX_OVERRUN,
    address_misaligned       = HAL_SD_ERROR_ADDR_MISALIGNED,
    invalid_block_length     = HAL_SD_ERROR_BLOCK_LEN_ERR,
    erase_sequence_error     = HAL_SD_ERROR_ERASE_SEQ_ERR,
    bad_erase_parameter      = HAL_SD_ERROR_BAD_ERASE_PARAM,
    write_protected          = HAL_SD_ERROR_WRITE_PROT_VIOLATION,
    card_locked              = HAL_SD_ERROR_LOCK_UNLOCK_FAILED,
    prev_command_crc_failed  = HAL_SD_ERROR_COM_CRC_FAILED,
    illegal_command          = HAL_SD_ERROR_ILLEGAL_CMD,
    card_ecc_failed          = HAL_SD_ERROR_CARD_ECC_FAILED,
    card_controller_error    = HAL_SD_ERROR_CC_ERR,
    unknown_error            = HAL_SD_ERROR_GENERAL_UNKNOWN_ERR,
    stream_read_underrun     = HAL_SD_ERROR_STREAM_READ_UNDERRUN,
    stream_write_underrun    = HAL_SD_ERROR_STREAM_WRITE_OVERRUN,
    cid_csd_overwrite_error  = HAL_SD_ERROR_CID_CSD_OVERWRITE,
    partially_erased         = HAL_SD_ERROR_WP_ERASE_SKIP,
    card_ecc_disabled        = HAL_SD_ERROR_CARD_ECC_DISABLED,
    erase_reset              = HAL_SD_ERROR_ERASE_RESET,
    authentication_error     = HAL_SD_ERROR_AKE_SEQ_ERR,
    invalid_voltage_range    = HAL_SD_ERROR_INVALID_VOLTRANGE,
    address_out_of_range     = HAL_SD_ERROR_ADDR_OUT_OF_RANGE,
    request_applicable       = HAL_SD_ERROR_REQUEST_NOT_APPLICABLE,
    invalid_parameter        = HAL_SD_ERROR_PARAM,
    unsupported_feature      = HAL_SD_ERROR_UNSUPPORTED_FEATURE,
    card_busys               = HAL_SD_ERROR_BUSY,
    dma_error                = HAL_SD_ERROR_DMA,
    timeout_error            = HAL_SD_ERROR_TIMEOUT
};

} // namespace sys::hal

template <>
struct sys::is_error_code_enum<sys::hal::hal_sdmmc_error> : std::true_type {};

namespace sys::hal {

static constexpr struct sdmmc_error_category : public sys::error_category
{
    constexpr sdmmc_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"HAL SDMMC"};  }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<hal_sdmmc_error>(value) )
            {
                case hal_sdmmc_error::no_error:
                    return "No error"sv;

                case hal_sdmmc_error::command_crc_failed:
                    return "Command response received (but CRC check failed)"sv;

                case hal_sdmmc_error::data_crc_failed:
                    return "Data block sent/received (CRC check failed)"sv;

                case hal_sdmmc_error::command_response_timeout:
                    return "Command response timeout"sv;

                case hal_sdmmc_error::data_response_timeout:
                    return "Data timeout"sv;

                case hal_sdmmc_error::tx_underrun:
                    return "Transmit FIFO underrun"sv;

                case hal_sdmmc_error::rx_overrun:
                    return "Receive FIFO overrun"sv;

                case hal_sdmmc_error::address_misaligned:
                    return "Misaligned address"sv;

                case hal_sdmmc_error::invalid_block_length:
                    return "Transferred block length is not allowed for the card or thex number of transferred bytes does not match the block length"sv;

                case hal_sdmmc_error::erase_sequence_error:
                    return "An error in the sequence of erase command occurs"sv;

                case hal_sdmmc_error::bad_erase_parameter:
                    return "An invalid selection for erase groups"sv;

                case hal_sdmmc_error::write_protected:
                    return "Attempt to program a write protect block"sv;

                case hal_sdmmc_error::card_locked:
                    return "sequence or password error has been detected in unlockxcommand or if there was an attempt to access a locked card"sv;

                case hal_sdmmc_error::prev_command_crc_failed:
                    return "CRC check of the previous command failed"sv;

                case hal_sdmmc_error::illegal_command:
                    return "Command is not legal for the card state"sv;

                case hal_sdmmc_error::card_ecc_failed:
                    return "Card internal ECC was applied but failed to correct the data"sv;

                case hal_sdmmc_error::card_controller_error:
                    return "Internal card controller error"sv;

                case hal_sdmmc_error::unknown_error:
                    return "General or unknown error"sv;

                case hal_sdmmc_error::stream_read_underrun:
                    return "The card could not sustain data reading in stream rmode"sv;

                case hal_sdmmc_error::stream_write_underrun:
                    return "The card could not sustain data programming in stream mode"sv;

                case hal_sdmmc_error::cid_csd_overwrite_error:
                    return "CID/CSD overwrite error"sv;

                case hal_sdmmc_error::partially_erased:
                    return "Only partial address space was erased"sv;

                case hal_sdmmc_error::card_ecc_disabled:
                    return "Command has been executed without using internal ECC"sv;

                case hal_sdmmc_error::erase_reset:
                    return "Erase sequence was cleared before executing because an out of erase sequence command was received"sv;

                case hal_sdmmc_error::authentication_error:
                    return "Error in sequence of authentication"sv;

                case hal_sdmmc_error::invalid_voltage_range:
                    return "Error in case of invalid voltage range"sv;

                case hal_sdmmc_error::address_out_of_range:
                    return "Error when addressed block is out of range"sv;

                case hal_sdmmc_error::request_applicable:
                    return "Error when command request is not applicable"sv;

                case hal_sdmmc_error::invalid_parameter:
                    return "the used parameter is not valid"sv;

                case hal_sdmmc_error::unsupported_feature:
                    return "Error when feature is not insupported"sv;

                case hal_sdmmc_error::card_busys:
                    return "Error when transfer process is busy"sv;

                case hal_sdmmc_error::dma_error:
                    return "Error while DMA transfer"sv;

                case hal_sdmmc_error::timeout_error:
                    return "Timeout error"sv;
            }
        }

        return {};
    }
} k_sdmmc_error_category{};

[[nodiscard]] constexpr auto make_error_code(hal_sdmmc_error error)
{
    return sys::error_code{error, k_sdmmc_error_category};
}

} // namespace sys::hal
