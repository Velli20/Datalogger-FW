#pragma once

extern "C" {

typedef struct lfs lfs_t;

} // extern "C"

namespace sys {

// Forward declarations.

namespace fatfs {
struct filesystem;
} // namespace fatfs

namespace littlefs {
struct filesystem;
} // namespace littlefs

// External declarations.

extern lfs_t*                g_littlefs_context;
extern fatfs::filesystem     g_fat_filesystem;
extern littlefs::filesystem  g_littlefs_filesystem;
extern std::filesystem::path g_current_path;

} // namespace sys
