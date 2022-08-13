#pragma once

namespace sys {

namespace {

template <typename T>
void filesystem_autocomplete_helper(
    std::string_view hint,
    std::span<char>  completion_buffer
) noexcept
{
    using namespace std::string_view_literals;
    using namespace sys::util;

    // Construct path.

    auto path = std::filesystem::path{""};

    // Open a directory with a given path.

    auto dir = T{};
    if ( dir.open(path) == false )
    {
        log::error() << dir.error_code() << log::endl;
        return;
    }

    std::size_t common_length[3]{0,0,0};

    // Read entries.

    while ( true )
    {
        auto entry = dir.read();
        if ( entry.type() == directory_entry_type::k_sentinel )
        {
            break;
        }

        auto name = entry.name();
        if ( name.empty() )
        {
            break;
        }

        // Find entries that start with the given hint.

        if ( std::strncmp(name.data(), hint.data(), hint.size()) != 0 )
        {
            continue;
        }

        common_length[0] = find_common_length(hint.data(), hint.size(), name.data(), name.size());
        if ( common_length[0] == common_length[1] )
        {
            continue;
        }

        // Increase the match count.

        common_length[2] += 1;

        // Update longest common lenght value.

        common_length[1] = std::min(common_length[0], completion_buffer.size());

        std::copy_n(std::cbegin(name), name.size(), std::begin(completion_buffer));
        completion_buffer.data()[name.size() + 1] = '\0';
    }

    if ( dir.close() == false )
    {
        log::error() << dir.error_code() << log::endl;
        return;
    }

    // In case of multiple matches cut the completion buffer length at the common length index.

    if ( common_length[2] > 1 )
    {
        completion_buffer.data()[common_length[1] + 1] = '\0';
    }
}

} // namespace

template <>
struct shell::command_type<"ls"> final
{
    static auto on_command_match(const shell::arguments&, log::formatting_stream<shell::input_line>& os)
    {
        using namespace std::string_view_literals;

        auto path = /* g_fat_filesystem.current_path() */std::filesystem::path{""};
        auto dir  = fatfs::directory{};

        if ( dir.open(path) == false )
        {
            os << dir.error_code() << log::endl;
            return;
        }

        os << "directory: \""sv << path.c_str() << "\""sv << " content"sv << log::endl;

        while ( true )
        {
            auto entry = dir.read();
            if ( entry.type() == directory_entry_type::k_sentinel )
            {
                break;
            }

            if ( entry.type() == directory_entry_type::k_directory )
            {
                os << entry.name() << '/' << log::endl;
            }

            else if ( entry.type() == directory_entry_type::k_regular_file )
            {
                os << entry.name() << log::endl;
            }

            else
            {
                os << entry.name() << " (Unknown entry type)" << log::endl;
            }
        }

        if ( dir.close() == false )
        {
            os << dir.error_code() << log::endl;
            return;
        }
    }

    static void on_try_complete_argument(std::string_view hint, std::span<char> completion_buffer)
    {
        std::memcpy(completion_buffer.data(), "Test", sizeof("Test"));
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "List files and directories."sv;
    }
};

template <>
struct shell::command_type<"cat"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace std::string_view_literals;

        auto path = std::filesystem::path{args[0].data()};

        auto handle = g_fat_filesystem.open(path, file_mode_type::k_read);
        if ( handle == nullptr )
        {
            os << g_fat_filesystem.error_code() << log::endl;
            return;
        }

        auto work_buffer = std::make_unique<std::byte[]>(512);
        debug_assert(work_buffer.get() != nullptr);

        auto bytes_total = handle->size();
        os << "file total size "sv << bytes_total << "bytes"sv << log::endl;

        for ( decltype(bytes_total) bytes_read{}; bytes_read < bytes_total; )
        {
            auto bytes_to_read = (bytes_total - bytes_read) > 512 ? 512 : (bytes_total - bytes_read);

            auto count = handle->read(std::span{work_buffer.get(), bytes_to_read});
            if ( count != bytes_to_read )
            {
                os << handle->error_code() << log::endl;
                return;
            }

            bytes_read += count;

            log::hexdump(std::span{work_buffer.get(), bytes_to_read});
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Print file content. <file name>"sv;
    }

    static void on_try_complete_argument(std::string_view hint, std::span<char> completion_buffer)
    {
        filesystem_autocomplete_helper<fatfs::directory>(hint, completion_buffer);
    }
};


template <>
struct shell::command_type<"mkdir"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        using namespace std::string_view_literals;

        if ( args.empty() == false )
        {
            auto path = std::filesystem::path{args[0]};
            if ( path.empty() )
            {
                os << "Empty path"sv << log::endl;
                return;
            }

            if ( g_fat_filesystem.create_directory(path) == false )
            {
                os << "Failed to create directory "sv << path.c_str() << log::endl;
            }
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Make directory <name>."sv;
    }
};

template <>
struct shell::command_type<"fatfs-format"> final
{
    static auto on_command_match()
    {
        using namespace std::string_view_literals;

        if ( auto success = g_fat_filesystem.format(); success == false )
        {
            log::error() << "Failed to format LittleFs."sv << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Format the SD-card."sv;
    }
};

template <>
struct shell::command_type<"littlefs-format"> final
{
    static auto on_command_match()
    {
        using namespace std::string_view_literals;

        if ( auto success = g_littlefs_filesystem.format(); success == false )
        {
            log::error() << "Failed to format LittleFs."sv << g_littlefs_filesystem.error_code() << log::endl;
            log::error() << g_littlefs_filesystem.error_code() << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Format LittleFs."sv;
    }
};

template <>
struct shell::command_type<"littlefs-test"> final
{
    static auto on_command_match(const shell::arguments&, log::formatting_stream<shell::input_line>& os)
    {
        using namespace std::string_view_literals;

        file* handle = g_littlefs_filesystem.open(std::filesystem::path{"test"}, file_mode_type::k_read_write_create_always);
        if ( handle == nullptr )
        {
            os << "file creation failed. "sv << g_littlefs_filesystem.error_code();
            return;
        }

        static constexpr auto k_test_string =
            R"(Lorem ipsum dolor sit amet, consectetur
              adipiscing elit. Aliquam eu molestie nisl.
              Nullam laoreet neque at sodales viverra.
              Duis faucibus imperdiet libero, eu sodales
              nisi iaculis vitae.)"sv;

        static constexpr auto k_test_data = std::span{k_test_string.data(), k_test_string.size()};

        os << "Testing to write "sv << k_test_data.size_bytes() << "bytes..."sv << log::endl;

        if ( auto n = handle->write(std::as_bytes(k_test_data)); n != k_test_data.size_bytes() )
        {
            os << "Write failed. "sv << handle->error_code() << log::endl;
            return;
        }

        handle->close();

        handle = g_littlefs_filesystem.open(std::filesystem::path{"test"sv}, file_mode_type::k_read);
        if ( handle == nullptr )
        {
            os << "file open failed. "sv << g_littlefs_filesystem.error_code();
            return;
        }

        if ( handle->size() != k_test_data.size_bytes() )
        {
            os << "file size("sv << handle->size() << ") failed. "sv << handle->error_code();
            return;
        }

        auto work_buffer = std::make_unique<std::byte[]>(512);
        debug_assert(work_buffer.get() != nullptr);

        auto bytes_total = k_test_data.size_bytes();
        os << "file total size "sv << bytes_total << "bytes"sv << log::endl;

        for ( decltype(bytes_total) bytes_read{}; bytes_read < bytes_total; )
        {
            auto bytes_to_read = (bytes_total - bytes_read) > 512 ? 512 : (bytes_total - bytes_read);

            auto count = handle->read(std::span{work_buffer.get(), bytes_to_read});
            if ( count != bytes_to_read )
            {
                os << handle->error_code() << log::endl;
                return;
            }

            bytes_read += count;

            log::hexdump(std::span{work_buffer.get(), bytes_to_read});
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Test LittleFs."sv;
    }
};

template <>
struct shell::command_type<"pwd"> final
{
    static auto on_command_match(const shell::arguments&, log::formatting_stream<shell::input_line>& os)
    {
        os << g_current_path.c_str();
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Returns current working directory."sv;
    }
};

template <>
struct shell::command_type<"cd"> final
{
    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        if ( args.empty() == false )
        {
            g_current_path /= args[0];
        }

    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Sets current working directory."sv;
    }
};

} // namespace sys
