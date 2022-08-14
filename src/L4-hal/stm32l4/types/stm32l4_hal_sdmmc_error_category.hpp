#pragma once

namespace sys::hal {

enum class hal_sdmmc_error_type : std::uint32_t
{
    k_no_error                 = HAL_SD_ERROR_NONE,
    k_command_crc_failed       = HAL_SD_ERROR_CMD_CRC_FAIL,
    k_data_crc_failed          = HAL_SD_ERROR_DATA_CRC_FAIL,
    k_command_response_timeout = HAL_SD_ERROR_CMD_RSP_TIMEOUT,
    k_data_response_timeout    = HAL_SD_ERROR_DATA_TIMEOUT,
    k_tx_underrun              = HAL_SD_ERROR_TX_UNDERRUN,
    k_rx_overrun               = HAL_SD_ERROR_RX_OVERRUN,
    k_address_misaligned       = HAL_SD_ERROR_ADDR_MISALIGNED,
    k_invalid_block_length     = HAL_SD_ERROR_BLOCK_LEN_ERR,
    k_erase_sequence_error     = HAL_SD_ERROR_ERASE_SEQ_ERR,
    k_bad_erase_parameter      = HAL_SD_ERROR_BAD_ERASE_PARAM,
    k_write_protected          = HAL_SD_ERROR_WRITE_PROT_VIOLATION,
    k_card_locked              = HAL_SD_ERROR_LOCK_UNLOCK_FAILED,
    k_prev_command_crc_failed  = HAL_SD_ERROR_COM_CRC_FAILED,
    k_illegal_command          = HAL_SD_ERROR_ILLEGAL_CMD,
    k_card_ecc_failed          = HAL_SD_ERROR_CARD_ECC_FAILED,
    k_card_controller_error    = HAL_SD_ERROR_CC_ERR,
    k_unknown_error            = HAL_SD_ERROR_GENERAL_UNKNOWN_ERR,
    k_stream_read_underrun     = HAL_SD_ERROR_STREAM_READ_UNDERRUN,
    k_stream_write_underrun    = HAL_SD_ERROR_STREAM_WRITE_OVERRUN,
    k_cid_csd_overwrite_error  = HAL_SD_ERROR_CID_CSD_OVERWRITE,
    k_partially_erased         = HAL_SD_ERROR_WP_ERASE_SKIP,
    k_card_ecc_disabled        = HAL_SD_ERROR_CARD_ECC_DISABLED,
    k_erase_reset              = HAL_SD_ERROR_ERASE_RESET,
    k_authentication_error     = HAL_SD_ERROR_AKE_SEQ_ERR,
    k_invalid_voltage_range    = HAL_SD_ERROR_INVALID_VOLTRANGE,
    k_address_out_of_range     = HAL_SD_ERROR_ADDR_OUT_OF_RANGE,
    k_request_applicable       = HAL_SD_ERROR_REQUEST_NOT_APPLICABLE,
    k_invalid_parameter        = HAL_SD_ERROR_PARAM,
    k_unsupported_feature      = HAL_SD_ERROR_UNSUPPORTED_FEATURE,
    k_card_busys               = HAL_SD_ERROR_BUSY,
    k_dma_error                = HAL_SD_ERROR_DMA,
    k_timeout_error            = HAL_SD_ERROR_TIMEOUT
};

} // namespace sys::hal

template <>
struct sys::is_error_code_enum<sys::hal::hal_sdmmc_error_type> : std::true_type {};

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

            switch ( static_cast<hal_sdmmc_error_type>(value) )
            {
                case hal_sdmmc_error_type::k_no_error:
                    return "No error"sv;

                case hal_sdmmc_error_type::k_command_crc_failed:
                    return "Command response received (but CRC check failed)"sv;

                case hal_sdmmc_error_type::k_data_crc_failed:
                    return "Data block sent/received (CRC check failed)"sv;

                case hal_sdmmc_error_type::k_command_response_timeout:
                    return "Command response timeout"sv;

                case hal_sdmmc_error_type::k_data_response_timeout:
                    return "Data timeout"sv;

                case hal_sdmmc_error_type::k_tx_underrun:
                    return "Transmit FIFO underrun"sv;

                case hal_sdmmc_error_type::k_rx_overrun:
                    return "Receive FIFO overrun"sv;

                case hal_sdmmc_error_type::k_address_misaligned:
                    return "Misaligned address"sv;

                case hal_sdmmc_error_type::k_invalid_block_length:
                    return "Transferred block length is not allowed for the card or thex number of transferred bytes does not match the block length"sv;

                case hal_sdmmc_error_type::k_erase_sequence_error:
                    return "An error in the sequence of erase command occurs"sv;

                case hal_sdmmc_error_type::k_bad_erase_parameter:
                    return "An invalid selection for erase groups"sv;

                case hal_sdmmc_error_type::k_write_protected:
                    return "Attempt to program a write protect block"sv;

                case hal_sdmmc_error_type::k_card_locked:
                    return "sequence or password error has been detected in unlockxcommand or if there was an attempt to access a locked card"sv;

                case hal_sdmmc_error_type::k_prev_command_crc_failed:
                    return "CRC check of the previous command failed"sv;

                case hal_sdmmc_error_type::k_illegal_command:
                    return "Command is not legal for the card state"sv;

                case hal_sdmmc_error_type::k_card_ecc_failed:
                    return "Card internal ECC was applied but failed to correct the data"sv;

                case hal_sdmmc_error_type::k_card_controller_error:
                    return "Internal card controller error"sv;

                case hal_sdmmc_error_type::k_unknown_error:
                    return "General or unknown error"sv;

                case hal_sdmmc_error_type::k_stream_read_underrun:
                    return "The card could not sustain data reading in stream rmode"sv;

                case hal_sdmmc_error_type::k_stream_write_underrun:
                    return "The card could not sustain data programming in stream mode"sv;

                case hal_sdmmc_error_type::k_cid_csd_overwrite_error:
                    return "CID/CSD overwrite error"sv;

                case hal_sdmmc_error_type::k_partially_erased:
                    return "Only partial address space was erased"sv;

                case hal_sdmmc_error_type::k_card_ecc_disabled:
                    return "Command has been executed without using internal ECC"sv;

                case hal_sdmmc_error_type::k_erase_reset:
                    return "Erase sequence was cleared before executing because an out of erase sequence command was received"sv;

                case hal_sdmmc_error_type::k_authentication_error:
                    return "Error in sequence of authentication"sv;

                case hal_sdmmc_error_type::k_invalid_voltage_range:
                    return "Error in case of invalid voltage range"sv;

                case hal_sdmmc_error_type::k_address_out_of_range:
                    return "Error when addressed block is out of range"sv;

                case hal_sdmmc_error_type::k_request_applicable:
                    return "Error when command request is not applicable"sv;

                case hal_sdmmc_error_type::k_invalid_parameter:
                    return "the used parameter is not valid"sv;

                case hal_sdmmc_error_type::k_unsupported_feature:
                    return "Error when feature is not insupported"sv;

                case hal_sdmmc_error_type::k_card_busys:
                    return "Error when transfer process is busy"sv;

                case hal_sdmmc_error_type::k_dma_error:
                    return "Error while DMA transfer"sv;

                case hal_sdmmc_error_type::k_timeout_error:
                    return "Timeout error"sv;
            }
        }

        return {};
    }
} k_sdmmc_error_category{};

[[nodiscard]] constexpr auto make_error_code(hal_sdmmc_error_type error)
{
    return sys::error_code{error, k_sdmmc_error_category};
}

} // namespace sys::hal
