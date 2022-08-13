#pragma once

// STD-library Includes.

// #if not __has_include("cmake_pch.hxx")
// #include <std/all.hpp>
// #endif

#if defined(NDEBUG)
#define debug_assert(x)          \
    do                           \
    {                            \
        static_cast<void>((x));  \
    }                            \
    while ( 0 )

#else
#define debug_assert(x) assert(x)
#endif /* NDEBUG */

#if __has_include(<firmware_config.hpp>)
#include <firmware_config.hpp>
#else
namespace sys::config::build_type {
    constexpr auto debug = true;
}

namespace sys::config::chan_fatfs {
    constexpr auto value = false;
}

namespace sys::config::littlefs {
    constexpr auto value = false;
}

namespace sys::config::freertos {
    constexpr auto value = false;
}
#endif

namespace sys {

struct error_code;

} // namespace sys

// External libraries.

#include <expected.hpp>


// Level 0 libraries.

#include <platform/platform_pmr.hpp>
#include <platform/platform_chrono.hpp>
#include <platform/platform_memory.hpp>
#include <platform/platform_stdio.hpp>

#include <util/util_concepts.hpp>
#include <util/util_math.hpp>
#include <util/util_countof.hpp>
#include <util/util_as_byte_data.hpp>
#include <util/util_hash.hpp>
#include <util/util_units.hpp>
#include <util/util_common_length.hpp>
#include <util/util_bitmask_enum.hpp>
#include <util/util_overloaded.hpp>

#include <algorithm/string_algorithm.hpp>

#include <meta/function_traits.hpp>

#include <string/static_string.hpp>

#include <interface/interface_file.hpp>


#include <device/interface/device_write_interface.hpp>
#include <device/device_node_name.hpp>
#include <device/peripheral_type.hpp>
#include <device/types/device_type_i2c_bus.hpp>
#include <device/types/device_type.hpp>
#include <device/config/device_config_base.hpp>
#include <device/device_socket.hpp>

#include <modem/response_table.hpp>

#include <stream/stream_concepts.hpp>
#include <stream/interface/input_sink_interface.hpp>
#include <stream/interface/output_sink_interface.hpp>

#include <stream/stream_stdfile.hpp> /* FIXME */
#include <stream/sink/stdio_stream_sink.hpp>
#include <stream/sink/file_stream_sink.hpp>
#include <stream/stream.hpp>

#include <logging/logging.hpp>
#include <mmio/types/register_types.hpp>
#include <mmio/concepts/register_concepts.hpp>
#include <mmio/register.hpp>

#include <result/error_category.hpp>
#include <result/posix_error_category.hpp>

#include <http/types/http_mime_type_type.hpp>
#include <http/types/http_status_code_type.hpp>
#include <http/types/http_method_type.hpp>
#include <http/types/http_start_line_type.hpp>
#include <http/types/http_protocol_type.hpp>
#include <http/types/http_field_type.hpp>
#include <http/types/http_url_components_type.hpp>
#include <http/types/http_response_body_type.hpp>
#include <http/types/http_response_type.hpp>
#include <http/types/http_header_type.hpp>
#include <http/types/http_message_type.hpp>
#include <http/types/http_request_view.hpp>

#include <http/concepts/http_router_concepts.hpp>

#include <http/parse/http_parse_header.hpp>
#include <http/parse/http_parse_url.hpp>

#include <http/http_router.hpp>
#include <http/http_factory.hpp>


#include <time/wait.hpp>

#include <ring_buffer/ring_buffer_iterator.hpp>
#include <ring_buffer/ring_buffer.hpp>

#include <memory/aligned_storage.hpp>
#include <memory/heap_allocator.hpp>
#include <memory/memory.hpp>

#include <shell/shell_arguments.hpp>
#include <shell/shell_prompt.hpp>
#include <shell/shell_command.hpp>
#include <shell/shell_command_view.hpp>
#include <shell/shell_concepts.hpp>
#include <shell/shell_command_list.hpp>
#include <shell/shell_parse_args.hpp>
#include <shell/shell_history.hpp>

#include <interface/interface_application.hpp>

#include <interface/interface_directory.hpp>
#include <interface/interface_filesystem.hpp>
#include <interface/interface_scheduler.hpp>
#include <interface/interface_socket.hpp>
