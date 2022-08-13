
// Includes.

#include <base.hpp>
#include <hal.hpp>
#include <devices.hpp>
#include <fatfs/diskio.h>

namespace sys {

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
extern "C" DSTATUS disk_status(BYTE pdrv)
{
    static_cast<void>(pdrv);

    return RES_OK;
}

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
extern "C" DSTATUS disk_initialize(BYTE pdrv)
{
    static_cast<void>(pdrv);

    return RES_OK;
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
extern "C" DRESULT disk_read(
    BYTE  pdrv,
    BYTE* buff,
    DWORD sector,
    UINT  count
)
{
    static_cast<void>(pdrv);

    using namespace sys::dev;

    const auto read_count = device_type<"/dev/disk@0">::read_blocks(std::span{buff, count}, static_cast<std::uint32_t>(sector));
    if ( read_count != count )
    {
        return RES_ERROR;
    }

    return RES_OK;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
extern "C" DRESULT disk_write(
    BYTE        pdrv,
    const BYTE* buff,
    DWORD       sector,
    UINT        count
)
{
    static_cast<void>(pdrv);

    using namespace sys::dev;

    const auto write_count = device_type<"/dev/disk@0">::write_blocks(std::span{buff, count}, static_cast<std::uint32_t>(sector));
    if ( write_count != count )
    {
        return RES_ERROR;
    }

    return RES_OK;
}

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
extern "C" DRESULT disk_ioctl(
    BYTE  pdrv,
    BYTE  cmd,
    void* buff
)
{
    static_cast<void>(pdrv);

    using namespace sys::dev;

    // Make sure that no pending write process.

    if ( cmd == CTRL_SYNC )
    {
        return RES_OK;
    }

    // Pass number of sectors on the disk.

    else if ( cmd == GET_SECTOR_COUNT )
    {
        DWORD* count_ptr = static_cast<DWORD*>(buff);
        *count_ptr       = static_cast<DWORD>(device_type<"/dev/disk@0">::block_count());
    }

    // Pass R/W sector size.

    else if ( cmd == GET_SECTOR_SIZE )
    {
        WORD* size_ptr = static_cast<WORD*>(buff);
        *size_ptr      = static_cast<WORD>(device_type<"/dev/disk@0">::block_size());
    }

    // Pass erase block size in unit of sector.

    else if ( cmd == GET_BLOCK_SIZE )
    {
        DWORD* size_ptr = static_cast<DWORD*>(buff);
        *size_ptr       = static_cast<DWORD>(device_type<"/dev/disk@0">::block_size());
    }

    return RES_OK;
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
extern "C" DWORD get_fattime(void)
{
    std::tm tim;

    using namespace sys::hal;

    // Get RTC timestamp.

    if ( hal::peripheral_type<"/chip/rtc">::get_time(tim) == true )
    {
        DWORD fattime = 0;

        fattime |= static_cast<DWORD>((tim.tm_year - 1980)  << 25);
        fattime |= static_cast<DWORD>(tim.tm_mon            << 21);
        fattime |= static_cast<DWORD>(tim.tm_mday           << 16);
        fattime |= static_cast<DWORD>(tim.tm_hour           << 11);
        fattime |= static_cast<DWORD>(tim.tm_min            << 5);
        fattime |= static_cast<DWORD>(tim.tm_sec             / 2);

        return fattime;
    }

    return 0;
}

// ff_memalloc

extern "C" void* ff_memalloc(std::size_t size)
{
    return std::malloc(size);
}

// ff_memfree

extern "C" void ff_memfree(void* ptr)
{
    std::free(ptr);
}

} // namespace sys
