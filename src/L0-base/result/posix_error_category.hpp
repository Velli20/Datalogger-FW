#pragma once

template <>
struct sys::is_error_code_enum<std::errc> : std::true_type {};

namespace sys::detail {

static constexpr struct posix_error_category : public sys::error_category
{
    constexpr posix_error_category()
        : error_category{} {}

    [[nodiscard]] constexpr std::string_view name() const noexcept override { return std::string_view{"POSIX"}; }

    [[nodiscard]] constexpr std::string_view message(std::int32_t value) const noexcept override
    {
        static_cast<void>(value);

        if constexpr ( sys::config::build_type::debug == true )
        {
            using namespace std::string_view_literals;

            switch ( static_cast<std::errc>(value) )
            {
                case std::errc::address_family_not_supported:
                    return "Address family not supported by protocol"sv;
                case std::errc::address_in_use:
                    return "Address already in use"sv;
                case std::errc::address_not_available:
                    return "Cannot assign requested address"sv;
                case std::errc::already_connected:
                    return "Transport endpoint is already connected"sv;
                case std::errc::argument_list_too_long:
                    return "Argument list too long"sv;
                case std::errc::argument_out_of_domain:
                    return "Numerical argument out of domain"sv;
                case std::errc::bad_address:
                    return "Bad address"sv;
                case std::errc::bad_file_descriptor:
                    return "Bad file descriptor"sv;
                case std::errc::bad_message:
                    return "Bad message"sv;
                case std::errc::broken_pipe:
                    return "Broken pipe"sv;
                case std::errc::connection_aborted:
                    return "Software caused connection abort"sv;
                case std::errc::connection_already_in_progress:
                    return "Operation already in progress"sv;
                case std::errc::connection_refused:
                    return "Connection refused"sv;
                case std::errc::connection_reset:
                    return "Connection reset by peer"sv;
                case std::errc::cross_device_link:
                    return "Invalid cross-device link"sv;
                case std::errc::destination_address_required:
                    return "Destination address required"sv;
                case std::errc::device_or_resource_busy:
                    return "Device or resource busy"sv;
                case std::errc::directory_not_empty:
                    return "Directory not empty"sv;
                case std::errc::executable_format_error:
                    return "Exec format error"sv;
                case std::errc::file_exists:
                    return "File exists"sv;
                case std::errc::file_too_large:
                    return "File too large"sv;
                case std::errc::filename_too_long:
                    return "File name too long"sv;
                case std::errc::function_not_supported:
                    return "Function not implemented"sv;
                case std::errc::host_unreachable:
                    return "No route to host"sv;
                case std::errc::identifier_removed:
                    return "Identifier removed"sv;
                case std::errc::illegal_byte_sequence:
                    return "Invalid or incomplete multibyte or wide character"sv;
                case std::errc::inappropriate_io_control_operation:
                    return "Inappropriate ioctl for device"sv;
                case std::errc::interrupted:
                    return "Interrupted system call"sv;
                case std::errc::invalid_argument:
                    return "Invalid argument"sv;
                case std::errc::invalid_seek:
                    return "Illegal seek"sv;
                case std::errc::io_error:
                    return "Input/output error"sv;
                case std::errc::is_a_directory:
                    return "Is a directory"sv;
                case std::errc::message_size:
                    return "Message too long"sv;
                case std::errc::network_down:
                    return "Network is down"sv;
                case std::errc::network_reset:
                    return "Network dropped connection on reset"sv;
                case std::errc::network_unreachable:
                    return "Network is unreachable"sv;
                case std::errc::no_buffer_space:
                    return "No buffer space available"sv;
                case std::errc::no_child_process:
                    return "No child processes"sv;
                case std::errc::no_link:
                    return "Link has been severed"sv;
                case std::errc::no_lock_available:
                    return "No locks available"sv;
                case std::errc::no_message:
                    return "No message of desired type"sv;
                case std::errc::no_protocol_option:
                    return "Protocol not available"sv;
                case std::errc::no_space_on_device:
                    return "No space left on device"sv;
                case std::errc::no_stream_resources:
                    return "Out of streams resources"sv;
                case std::errc::no_such_device_or_address:
                    return "No such device or address"sv;
                case std::errc::no_such_device:
                    return "No such device"sv;
                case std::errc::no_such_file_or_directory:
                    return "No such file or directory"sv;
                case std::errc::no_such_process:
                    return "No such process"sv;
                case std::errc::not_a_directory:
                    return "Not a directory"sv;
                case std::errc::not_a_socket:
                    return "Socket operation on non-socket"sv;
                case std::errc::not_a_stream:
                    return "Device not a stream"sv;
                case std::errc::not_connected:
                    return "Transport endpoint is not connected"sv;
                case std::errc::not_enough_memory:
                    return "Cannot allocate memory"sv;
            #if ENOTSUP != EOPNOTSUPP
                case std::errc::not_supported:
                    return "Operation not supported"sv;
            #endif
                case std::errc::operation_canceled:
                    return "Operation canceled"sv;
                case std::errc::operation_in_progress:
                    return "Operation now in progress"sv;
                case std::errc::operation_not_permitted:
                    return "Operation not permitted"sv;
                case std::errc::operation_not_supported:
                    return "Operation not supported"sv;
            #if EAGAIN != EWOULDBLOCK
                case std::errc::operation_would_block:
                    return "Resource temporarily unavailable"sv;
            #endif
                case std::errc::owner_dead:
                    return "Owner died"sv;
                case std::errc::permission_denied:
                    return "Permission denied"sv;
                case std::errc::protocol_error:
                    return "Protocol error"sv;
                case std::errc::protocol_not_supported:
                    return "Protocol not supported"sv;
                case std::errc::read_only_file_system:
                    return "Read-only file system"sv;
                case std::errc::resource_deadlock_would_occur:
                    return "Resource deadlock avoided"sv;
                case std::errc::resource_unavailable_try_again:
                    return "Resource temporarily unavailable"sv;
                case std::errc::result_out_of_range:
                    return "Numerical result out of range"sv;
                case std::errc::state_not_recoverable:
                    return "State not recoverable"sv;
                case std::errc::stream_timeout:
                    return "Timer expired"sv;
                case std::errc::text_file_busy:
                    return "Text file busy"sv;
                case std::errc::timed_out:
                    return "Connection timed out"sv;
                case std::errc::too_many_files_open_in_system:
                    return "Too many open files in system"sv;
                case std::errc::too_many_files_open:
                    return "Too many open files"sv;
                case std::errc::too_many_links:
                    return "Too many links"sv;
                case std::errc::too_many_symbolic_link_levels:
                    return "Too many levels of symbolic links"sv;
                case std::errc::value_too_large:
                    return "Value too large for defined data type"sv;
                case std::errc::wrong_protocol_type:
                    return "Protocol wrong type for socket"sv;

                default:
                    return "Unspecified error"sv;
            }
        }

        return {};
    }

} k_posix_error_category{};

} // namespace sys::detail

namespace sys {

[[nodiscard]] constexpr auto make_error_code(std::errc error)
{
    return sys::error_code{error, detail::k_posix_error_category};
}

} // namespace sys
