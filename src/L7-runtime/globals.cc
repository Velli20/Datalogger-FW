
// Includes.

#include <base.hpp>
#include <globals.hpp>
#include <hal.hpp>
#include <devices.hpp>
#include <filesystems.hpp>

namespace sys {

// Global variables.

constinit lfs_t*                g_littlefs_context{};
constinit fatfs::filesystem     g_fat_filesystem{};
constinit littlefs::filesystem  g_littlefs_filesystem{};
std::filesystem::path           g_current_path{};

} // namespace sys
