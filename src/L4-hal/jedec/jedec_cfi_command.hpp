#pragma once

namespace sys::hal::jedec {

// The Common Flash Memory Interface instruction set.

enum class cfi_command : std::uint8_t
{
    k_write_enable                = 0x06,
    k_write_disable               = 0x04,
    k_read_status_register1       = 0x05,
    k_read_status_register2       = 0x35,
    k_read_status_register3       = 0x15,
    k_write_status_register1      = 0x01,
    k_write_status_register2      = 0x31,
    k_write_status_register3      = 0x11,
    k_read_data                   = 0x03,
    k_read_data_fast              = 0x0B,
    k_read_data_dual_output       = 0x3B,
    k_read_data_dual_io_fast      = 0xBB,
    k_read_data_quad_output_fast  = 0x6B,
    k_read_data_quad_io_fast      = 0xEB,
    k_read_data_quad_io_word_fast = 0xE7,
    k_page_program                = 0x02,
    k_quad_page_program           = 0x32,
    k_sector_erase                = 0x20,
    k_block_erase32k              = 0x52,
    k_block_erase64k              = 0xD8,
    k_chip_erase                  = 0xC7,
    k_enable_reset                = 0x66,
    reset                         = 0x99,
    k_set_burst_with_wrap         = 0x77,
    k_program_erase_suspend       = 0x75,
    k_program_erase_resume        = 0x7A,
    k_release_from_deep_power_down= 0xAB,
    k_deep_power_down             = 0xB9,
    k_jedec_id                    = 0x9F,
    k_jedec_sfdp                  = 0x5A,
    k_unique_id                   = 0x4B,
};

} // namespace sys::hal::jedec
