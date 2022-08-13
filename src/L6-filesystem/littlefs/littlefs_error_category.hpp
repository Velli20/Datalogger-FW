
#pragma once

// Includes.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <littlefs/lfs.h>
#pragma GCC diagnostic pop

template <>
struct sys::is_error_code_enum<lfs_error> : std::true_type {};

namespace sys::littlefs {

static constexpr struct littlefs_error_category : public sys::error_category
{
    constexpr littlefs_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"LittleFS"}; }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<lfs_error>(value) )
            {
                case lfs_error::LFS_ERR_OK:
                    return "No error."sv;

                case lfs_error::LFS_ERR_IO:
                    return "Error during device operation."sv;

                case lfs_error::LFS_ERR_CORRUPT:
                    return "Corrupted."sv;

                case lfs_error::LFS_ERR_NOENT:
                    return "No directory entry."sv;

                case lfs_error::LFS_ERR_EXIST:
                    return "Entry already exists."sv;

                case lfs_error::LFS_ERR_NOTDIR:
                    return "Entry is not a dir."sv;

                case lfs_error::LFS_ERR_ISDIR:
                    return "Entry is a dir."sv;

                case lfs_error::LFS_ERR_NOTEMPTY:
                    return "Dir is not empty."sv;

                case lfs_error::LFS_ERR_BADF:
                    return "Bad file number."sv;

                case lfs_error::LFS_ERR_FBIG:
                    return "file too large."sv;

                case lfs_error::LFS_ERR_INVAL:
                    return "Invalid parameter."sv;

                case lfs_error::LFS_ERR_NOSPC:
                    return "No space left on device."sv;

                case lfs_error::LFS_ERR_NOMEM:
                    return "No more memory available."sv;

                case lfs_error::LFS_ERR_NOATTR:
                    return "No data/attr available."sv;

                case lfs_error::LFS_ERR_NAMETOOLONG:
                    return "file name too long."sv;
            }
        }

        return {};
    }
} k_littlefs_category{};

[[nodiscard]] constexpr auto make_error_code(lfs_error error)
{
    return sys::error_code{error, k_littlefs_category};
}

} // namespace sys::littlefs

