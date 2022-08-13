#pragma once

namespace sys {

struct filesystem
{
    // enum class copy_options {
    // none = /* unspecified */,
    // skip_existing = /* unspecified */,
    // overwrite_existing = /* unspecified */,
    // update_existing = /* unspecified */,
    // recursive = /* unspecified */,
    // copy_symlinks = /* unspecified */,
    // skip_symlinks = /* unspecified */,
    // directories_only = /* unspecified */,
    // create_symlinks = /* unspecified */,
    // create_hard_links = /* unspecified */
    // };

    // std::filesystem:copy("sandbox/file1.txt", "sandbox/file2.txt"); // copy file
    // std::filesystem:copy("sandbox/dir", "sandbox/dir2"); // copy directory (non-recursive)
    // const auto copyOptions = std::filesystem::copy_options::update_existing
    //                        | std::filesystem::copy_options::recursive
    //                        | std::filesystem::copy_options::directories_only
    // std::filesystem::copy("sandbox", "sandbox_copy", copyOptions);

    using path_type = std::filesystem::path;

    constexpr filesystem() noexcept         = default;
    constexpr filesystem(const filesystem&) = delete;
    constexpr filesystem(filesystem&&)      = delete;

    [[maybe_unused]] constexpr auto operator=(filesystem&&)      noexcept -> filesystem& = delete;

    [[maybe_unused]] constexpr auto operator=(const filesystem&) noexcept -> filesystem& = delete;

    virtual bool            format()                                   noexcept = 0;
    virtual bool            mount()                                    noexcept = 0;
    virtual bool            unmount()                                  noexcept = 0;
    virtual bool            create_directory(const path_type&)         noexcept = 0;
    virtual bool            unlink(const path_type&)                   noexcept = 0;
    virtual bool            rename(const path_type&, const path_type&) noexcept = 0;
    virtual path_type       current_path()                             noexcept = 0;
    virtual path_type       current_path(const path_type&)             noexcept = 0;
    virtual file*           open(const path_type&, file_mode_type)          noexcept = 0;
    virtual sys::error_code error_code()                               noexcept = 0;

    /* TODO:
    virtual bool Mount(struct BlockDevice)  noexcept = 0;
    */
};

} // namespace sys
