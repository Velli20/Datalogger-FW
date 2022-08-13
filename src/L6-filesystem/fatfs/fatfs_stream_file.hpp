#pragma once

namespace sys::log {

[[nodiscard]] inline auto fatfs(const char* path)
{
    auto* h = g_fat_filesystem.open(std::filesystem::path{path}, file_mode_type::k_append);
    debug_assert(h != nullptr);

    return formatting_stream{sys::io::file_stream_sink{*h}};
}

} // namespace sys::log

