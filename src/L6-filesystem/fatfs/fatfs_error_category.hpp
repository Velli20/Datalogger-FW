
#pragma once

#include <fatfs/ff.h>

template <>
struct sys::is_error_code_enum<FRESULT> : std::true_type {};

namespace sys::fatfs {

static constexpr struct fatfs_error_category : public sys::error_category
{
    constexpr fatfs_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"FATFS"}; }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<FRESULT>(value) )
            {
                case FRESULT::FR_OK:
                    return "Succeeded."sv;

                case FRESULT::FR_DISK_ERR:
                    return "A hard error occurred in the low level disk I/O layer."sv;

                case FRESULT::FR_INT_ERR:
                    return "Assertion failed."sv;

                case FRESULT::FR_NOT_READY:
                    return "The physical drive cannot work."sv;

                case FRESULT::FR_NO_FILE:
                    return "Could not find the file."sv;

                case FRESULT::FR_NO_PATH:
                    return "Could not find the path."sv;

                case FRESULT::FR_INVALID_NAME:
                    return "The path name format is invalid."sv;

                case FRESULT::FR_DENIED:
                    return "Access denied due to prohibited access or directory full."sv;

                case FRESULT::FR_EXIST:
                    return "Access denied due to prohibited access."sv;

                case FRESULT::FR_INVALID_OBJECT:
                    return "The file/directory object is invalid."sv;

                case FRESULT::FR_WRITE_PROTECTED:
                    return "The physical drive is write protected."sv;

                case FRESULT::FR_INVALID_DRIVE:
                    return "The logical drive number is invalid."sv;

                case FRESULT::FR_NOT_ENABLED:
                    return "The volume has no work area."sv;

                case FRESULT::FR_NO_FILESYSTEM:
                    return "There is no valid FAT volume."sv;

                case FRESULT::FR_MKFS_ABORTED:
                    return "The f_mkfs() aborted due to any problem."sv;

                case FRESULT::FR_TIMEOUT:
                    return "Could not get a grant to access the volume within defined period."sv;

                case FRESULT::FR_LOCKED:
                    return "The operation is rejected according to the file sharing policy."sv;

                case FRESULT::FR_NOT_ENOUGH_CORE:
                    return "LFN working buffer could not be allocated."sv;

                case FRESULT::FR_TOO_MANY_OPEN_FILES:
                    return "Number of open files > _FS_LOCK."sv;

                case FRESULT::FR_INVALID_PARAMETER:
                    return "Given parameter is invalid."sv;
            }
        }

        return {};
    }
} k_fatfs_error_category{};

[[nodiscard]] constexpr auto make_error_code(FRESULT error)
{
    return sys::error_code{error, k_fatfs_error_category};
}

} // namespace sys::fatfs

