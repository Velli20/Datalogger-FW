#pragma once

namespace sys {

template <>
struct os::static_task<"Sensor"> final : public os::task_base<2048, os::priority_type::k_priority_medium>
{
    public:

    static constexpr os::signal_type k_signal_read_values       = 1 << 6;
    static constexpr os::signal_type k_signal_userled_on        = 1 << 7;
    static constexpr os::signal_type k_signal_userled_off       = 1 << 8;
    static constexpr os::signal_type k_signal_userled_toggle    = 1 << 9;

    private:

    static constexpr os::signal_type k_signal_joystick_up       = 1 << 1;
    static constexpr os::signal_type k_signal_joystick_down     = 1 << 2;
    static constexpr os::signal_type k_signal_joystick_left     = 1 << 3;
    static constexpr os::signal_type k_signal_joystick_right    = 1 << 4;
    static constexpr os::signal_type k_signal_joystick_center   = 1 << 5;

    static constexpr os::signal_type k_expected_signals =
        k_signal_read_values      |
        k_signal_userled_on       |
        k_signal_userled_off      |
        k_signal_userled_toggle   |
        k_signal_joystick_up      |
        k_signal_joystick_down    |
        k_signal_joystick_left    |
        k_signal_joystick_right   |
        k_signal_joystick_center;

    private:

    struct sensor_web_server
    {
        template <string::static_string k_path>
        static auto on(const auto& req) noexcept -> http::router_response_type requires(k_path == std::string_view{R"(/sns)"})
        {
            log::info() << "Hello from /sns" << log::endl;

            for ( const auto& [name, value] : req.fields() )
            {
                log::info() << "{" << name << ", " << value << '}' << log::endl;
            }

            auto s = platform::pmr::string{R"(<!doctype html><html lang=en><title>Error 404: Resource not found</title>)", platform::memory::default_resource()};

            return http::response{http::status_code_type::k_ok, std::move(s)};
        }

        template <string::static_string k_path>
        static auto on(const auto& req) noexcept -> http::router_response_type requires(k_path == std::string_view{R"(/files)"})
        {
            log::info() << "Hello from /files" << log::endl;

            return http::response{http::status_code_type::k_ok, std::filesystem::path{"/some/path.txt"}};
        }

        template <string::static_string k_path>
        static auto on(const auto& req) noexcept -> http::router_response_type requires(k_path == std::string_view{R"(/test)"})
        {
            log::info() << "Hello from /test" << log::endl;

            for ( const auto& [name, value] : req.fields() )
            {
                log::info() << "{" << name << ", " << value << '}' << log::endl;
            }

            constexpr auto k_post = std::string_view
            {
                R"(<!DOCTYPE html>)"
                R"(<html lang="en">)"
                R"(<form id="formElem">)"
                R"(  <input type="file" name="image_file" accept="image/*">)"
                R"(  <input type="file" name="text_file" accept="text/*">)"
                R"(  <input type="submit">)"
                R"(</form>)"
                R"(<script>)"
                R"(  formElem.onsubmit = async (e) => {)"
                R"(    e.preventDefault();)"
                R"(    let res = await fetch('/post', {)"
                R"(      method: 'POST',)"
                R"(      body: new FormData(formElem))"
                R"(    });)"
                R"(    console.log(await res.text());)"
                R"(  };)"
                R"(</script>)"
                R"(</html>)"
            };

            return http::response_view{http::status_code_type::k_ok, k_post};
        }

        template <string::static_string k_path>
        static auto on(const auto& req) noexcept -> http::router_response_type requires(k_path == std::string_view{R"(/post)"})
        {
            auto os = log::info() << "Hello from /post. Got:" << log::endl;

            for ( const auto& [name, value] : req.fields() )
            {
                os << "{" << name << ", " << value << '}' << log::endl;
            }

            if ( req.has_body() )
            {
                os << log::endl << "begin of body" << log::endl;
                for ( auto c : req.body() )
                {
                    if ( c == std::byte{} )
                    {
                        os << "(empty byte)" << log::endl;
                    }
                    else
                    {
                        os << std::to_integer<char>(c);
                    }
                }
                os << log::endl << "end of body" << log::endl;
            }

            return http::response_view{http::status_code_type::k_ok};
        }
    };

    public:

    static void input_handler(
        dev::input_event   event,
        dev::key_code_type code
    ) noexcept
    {
        static_cast<void>(event);

        switch ( code )
        {
            case dev::key_code_type::k_key_code_up:
                os::scheduler::signal<"Sensor">(k_signal_joystick_up);
                break;

            case dev::key_code_type::k_key_code_down:
                os::scheduler::signal<"Sensor">(k_signal_joystick_down);
                break;

            case dev::key_code_type::k_key_code_left:
                os::scheduler::signal<"Sensor">(k_signal_joystick_left);
                break;

            case dev::key_code_type::k_key_code_right:
                os::scheduler::signal<"Sensor">(k_signal_joystick_right);
                break;

            case dev::key_code_type::k_key_code_center:
                os::scheduler::signal<"Sensor">(k_signal_joystick_center);
                break;

            case dev::key_code_type::k_none:
                [[fallthrough]];

            default:
                break;
        }
    }

    inline static /* constexpr */ void run(void*)
    {
        using namespace sys::hal;
        using namespace sys::dev;
        using namespace std::chrono_literals;

        device_type<"/dev/joystick@0">::init(input_handler);

        static constexpr auto server = http::make_router<sensor_web_server, R"(/sns)", R"(/test)", R"(/files)", R"(/post)">();

        device_type<"/dev/wifi@0">::listen(server);

        while ( true )
        {
            // Wait for signals.

            const auto signal = this_thread::wait(k_expected_signals, 500ms);
            if ( signal.has_value() != true )
                continue;

            if ( (signal.value() & k_signal_joystick_up) )
            {
                log::info() << "Joystick up" << log::endl;
            }

            if ( (signal.value() & k_signal_joystick_down) )
            {
                log::info() << "Joystick down" << log::endl;
            }

            if ( (signal.value() & k_signal_joystick_left) )
            {
                log::info() << "Joystick left" << log::endl;
            }

            if ( (signal.value() & k_signal_joystick_right) )
            {
                log::info() << "Joystick right" << log::endl;
            }

            if ( (signal.value() & k_signal_joystick_center) )
            {
                log::info() << "Joystick center" << log::endl;

                std::tm tim;
                hal::peripheral_type<"/chip/rtc">::get_time(tim);

                auto ss = log::fatfs("joystick.txt");
                ss << tim << " Joystick: Center" << log::endl;
            }

            {
                // LED on signal.

                if ( (signal.value() & k_signal_userled_on) )
                {
                    device_type<"/dev/led@1">::on();
                }

                // LED off signal.

                if ( (signal.value() & k_signal_userled_off) )
                {
                    device_type<"/dev/led@1">::off();
                }

                if ( (signal.value() & k_signal_userled_toggle) )
                {
                    device_type<"/dev/led@1">::toggle();
                }
            }
        }
    }
};

} // namespace sys
