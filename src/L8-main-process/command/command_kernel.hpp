#pragma once

namespace sys {

template <>
struct shell::command_type<"kernel-stats"> final
{
    static constexpr const auto k_header_top = std::string_view
    {
        "|    Task Name     |   State   |  CPU%  | Stack Left |   Priority  |"
    };

    static constexpr const auto k_header_bottom = std::string_view
    {
        "|                  |           |        |  in words  | Base : Curr |"
    };

    static constexpr const auto k_divider = std::string_view
    {
        "+------------------+-----------+--------+------------+-------------+"
    };

    static constexpr const auto k_format = std::string_view
    {
        "| %16.16s | %9.9s |  %3u %% | %10hu | %4lu : %-4lu |"
    };

    static constexpr std::size_t k_stats_length = 128;

    static constexpr const char* rtos_state_to_string(eTaskState state)
    {
        switch ( state )
        {
            case eTaskState::eBlocked:   return "BLOCKED";
            case eTaskState::eDeleted:   return "DELETED";
            case eTaskState::eInvalid:   return "INVALID";
            case eTaskState::eReady:     return "READY";
            case eTaskState::eRunning:   return "RUNNING";
            case eTaskState::eSuspended: return "SUSPENDED";
            default:                     return "UNKNOWN";
        }
    }

    static auto on_command_match(const shell::arguments& args, log::formatting_stream<shell::input_line>& os)
    {
        const auto buffer = std::make_unique<char[]>(k_stats_length);
        if ( buffer.get() == nullptr )
        {
            return;
        }

        TaskStatus_t task_status[32];
        std::uint32_t total_runtime;

        const auto elapsed_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(platform::chrono::high_resolution_clock::now());
        const auto epoch      = elapsed_ms.time_since_epoch();
        const auto value      = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

        os << log::fmt("Uptime %d ms", value.count()) << log::endl;

        // Generate raw status information about each task.

        const auto number_of_tasks = uxTaskGetSystemState(task_status, std::size(task_status), std::addressof(total_runtime));

        // For percentage calculations.

        total_runtime /= 100UL;

        os << k_divider       << log::endl;
        os << k_header_top    << log::endl;
        os << k_header_bottom << log::endl;

        // format the raw data as human readable ASCII data.

        for ( std::size_t x{}; x != number_of_tasks; ++x )
        {
            std::uint32_t cpu_utilization_percent = task_status[x].ulRunTimeCounter / total_runtime;

            os << k_divider << log::endl;
            os << log::fmt(k_format.data(),
                           task_status[x].pcTaskName,
                           rtos_state_to_string(task_status[x].eCurrentState),
                           cpu_utilization_percent,
                           task_status[x].usStackHighWaterMark,
                           task_status[x].uxBasePriority,
                           task_status[x].uxCurrentPriority) << log::endl;
        }

        os << k_divider << log::endl;
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Print run-time stats."sv;
    }
};

} // namespace sys
