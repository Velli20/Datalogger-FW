
// Includes.

#include <base.hpp>
#include <hal.hpp>
#include <devices.hpp>

#include "tusb.h"
#include "usb_descriptor.hpp"

namespace sys {

// Invoked when received SCSI_CMD_INQUIRY
// Application fill vendor id, product id and revision with string up to 8, 16, 4 characters respectively

extern "C" void tud_msc_inquiry_cb(std::uint8_t lun,
                                   std::uint8_t vendor_id[8],
                                   std::uint8_t product_id[16],
                                   std::uint8_t product_rev[4])
{
    using namespace sys::dev;

    static_cast<void>(lun);

    constexpr auto kVendor          = std::string_view{"TinyUSB"};
    constexpr auto kProductId       = std::string_view{"Mass Storage"};
    constexpr auto kProductRevision = std::string_view{"1.0"};

    constexpr auto copy_span = [] <typename T> (std::span<T> s0, const std::string_view s1)
    {
        std::memcpy(s0.data(), s1.data(), std::min(s0.size(), s1.size()));
    };

    copy_span(std::span{vendor_id,   8u}, kVendor);
    copy_span(std::span{product_id, 16u}, kProductId);
    copy_span(std::span{product_rev, 4u}, kProductRevision);
}

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted

extern "C" bool tud_msc_test_unit_ready_cb(std::uint8_t lun)
{
    using namespace sys::dev;

    return device_type<"/dev/disk@0">::is_ready(lun);
}

// Invoked when received SCSI_CMD_READ_CAPACITY_10 and SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size
// Application update block count and block size

extern "C" void tud_msc_capacity_cb(std::uint8_t lun, std::uint32_t* block_count, std::uint16_t* block_size)
{
    static_cast<void>(lun);

    using namespace sys::dev;

    *block_count = device_type<"/dev/disk@0">::block_count();
    *block_size  = device_type<"/dev/disk@0">::block_size();
}

// Invoked when received Start Stop Unit command
// - Start = 0 : stopped power mode, if load_eject = 1 : unload disk storage
// - Start = 1 : active mode, if load_eject = 1 : load disk storage

extern "C" bool tud_msc_start_stop_cb(std::uint8_t lun,
                                      std::uint8_t power_condition,
                                      bool         start,
                                      bool         load_eject)
{
    static_cast<void>(lun);
    static_cast<void>(power_condition);

    if ( load_eject )
    {
        // load disk storage
        if ( start )
        {
            log::info() << "[USB] disk load" << log::endl;
        }

        // unload disk storage
        else
        {
            log::info() << "[USB] disk unload" << log::endl;
        }
    }

    return true;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and return number of copied bytes.

extern "C" std::int32_t tud_msc_read10_cb(std::uint8_t  lun,
                                          std::uint32_t block_address,
                                          std::uint32_t offset,
                                          void*         buffer,
                                          std::uint32_t bufsize)
{
    static_cast<void>(lun);
    static_cast<void>(offset);

    using namespace sys::dev;

    const auto block_size  = device_type<"/dev/disk@0">::block_size();
    const auto block_count = static_cast<std::size_t>(bufsize / block_size);
    const auto block_buf   = reinterpret_cast<std::byte*>(buffer);

    const auto count = device_type<"/dev/disk@0">::read_blocks(std::span(block_buf, block_count), block_address);
    if ( count != block_count )
    {
        sys::log::error() << "[USB] disk read failed [count(" << count << ") != block_count(" << block_count << ")" << sys::log::endl;
        return -1;
    }

    return static_cast<std::int32_t>(bufsize);
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and return number of written bytes

extern "C" std::int32_t tud_msc_write10_cb(std::uint8_t  lun,
                                           std::uint32_t block_address,
                                           std::uint32_t offset,
                                           std::uint8_t* buffer,
                                           std::uint32_t bufsize)
{
    static_cast<void>(lun);
    static_cast<void>(offset);

    using namespace sys::dev;

    const auto block_size  = device_type<"/dev/disk@0">::block_size();
    const auto block_count = static_cast<std::size_t>(bufsize / block_size);
    const auto block_buf   = reinterpret_cast<const std::byte*>(buffer);

    const auto count = device_type<"/dev/disk@0">::write_blocks(std::span(block_buf, block_count), block_address);
    if ( count != block_count )
    {
        sys::log::error() << "[USB] disk read failed [count(" << count << ") != block_count(" << block_count << ")" << sys::log::endl;
        return -1;
    }

    return static_cast<std::int32_t>(bufsize);
}

// Callback invoked when received an SCSI command not in built-in list below
// - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, MODE_SENSE6, REQUEST_SENSE
// - READ10 and WRITE10 has their own callbacks

extern "C" int32_t tud_msc_scsi_cb(std::uint8_t       lun,
                                   std::uint8_t const scsi_cmd[16],
                                   void*              buffer,
                                   std::uint16_t      bufsize)
{
    // read10 & write10 has their own callback and MUST not be handled here

    void const*  response{nullptr};
    std::int32_t resplen{};

    // most scsi handled is input
    bool in_xfer{true};

    switch ( scsi_cmd[0] )
    {
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            // Host is about to read/write etc ... better not to disconnect disk
            resplen = 0;
            break;

        default:
            // Set Sense = Invalid Command Operation
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

            // negative means error -> tinyusb could stall and/or response with failed status
            resplen = -1;
            break;
    }

    // return resplen must not larger than bufsize.

    if ( resplen > bufsize )
    {
        resplen = bufsize;
    }

    if ( response && (resplen > 0) )
    {
        if ( in_xfer )
        {
            memcpy(buffer, response, static_cast<std::size_t>(resplen));
        }

        else
        {
            // SCSI output
        }
    }

    return resplen;
}

} // namespace sys
