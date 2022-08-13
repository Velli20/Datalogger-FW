
// Includes.

#include <base.hpp>
#include <littlefs_trace.h>
#include <stdarg.h>

extern "C" void lfs_trace_impl(const char* file, size_t line, const char* fmt, ...)
{
    static_cast<void>(file);
    static_cast<void>(line);
    static_cast<void>(fmt);

    if constexpr ( sys::config::build_type::debug == true )
    {
        char buffer[256];

        va_list args;
        va_start(args, fmt);

        std::snprintf(buffer, sizeof(buffer), fmt, args);
        sys::log::error() << buffer << sys::log::endl;

        va_end(args);
    }
}

extern "C" void* lfs_malloc(size_t size)
{
    sys::log::info() << "[LFS] malloc " << size << sys::log::endl;

    return malloc(size);
}

extern "C" void lfs_free(void* p)
{
    free(p);
}

extern "C" uint32_t lfs_crc(uint32_t crc, const void* buffer, size_t size)
{
    static const uint32_t rtable[16] =
    {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
    };

    const uint8_t* data = reinterpret_cast<const uint8_t*>(buffer);

    for (size_t i = 0; i < size; i++) {
        crc = (crc >> 4) ^ rtable[(crc ^ (data[i] >> 0)) & 0xf];
        crc = (crc >> 4) ^ rtable[(crc ^ (data[i] >> 4)) & 0xf];
    }

    return crc;
}

