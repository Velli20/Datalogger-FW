#pragma once

namespace sys {

template <>
struct shell::command_type<"test-arm-io-illegal-instruction"> final
{
    static auto on_command_match()
    {
        using FUN = int(*)(void);

        FUN f = (FUN) 0xE0000000;
        f();
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Test instruction acceess violation."sv;
    }
};

template <>
struct shell::command_type<"test-arm-div-zero-trap"> final
{

#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wdiv-by-zero"

    static auto on_command_match()
    {
        using namespace std::string_view_literals;

        std::uint32_t result = 4U / 0U;

        log::info() << "Divide by zero "sv << result << log::endl;
    }

#   pragma GCC diagnostic pop

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Test 'Divide By Zero' trap."sv;
    }
};

template <>
struct shell::command_type<"test-arm-float-access"> final
{
    static auto on_command_match()
    {
        // FreeRTOS will automatically enable the FPU co-processor.
        // Let's disable it for the purposes of this example

        asm volatile(
        R"(
            ldr r0, =0xE000ED88
            mov r1, #0
            str r1, [r0]
            dsb
            vmov r0, s0
        )");
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Test FPU access in user mode."sv;
    }
};

template <>
struct shell::command_type<"test-arm-bad-address"> final
{
    static auto on_command_match()
    {
        using namespace std::string_view_literals;

        volatile std::uint32_t* invalid_address = reinterpret_cast<volatile std::uint32_t*>(0xDEADBEEF);

        if ( *invalid_address )
        {
            log::info() << "ok"sv << log::endl;
        }
    }

    static consteval auto description()
    {
        using namespace std::string_view_literals;

        return "Read from a bad address"sv;
    }
};

} // namespace sys
