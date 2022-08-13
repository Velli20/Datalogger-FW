
// Includes.

#include <base.hpp>
#include <board_impl.hpp>
#include <devices.hpp>

namespace {

// Vector table.

[[gnu::section(".vector_table"), gnu::used]]
constexpr auto k_vector_table = sys::hal::vector_table<sys::util::count_of(sys::hal::k_interrupt_map), sys::hal::k_interrupt_map>{};

// Reserve storage for the application memory resource.

[[gnu::section(".noinit")]]
static sys::type_storage<sys::heap_allocator> s_pmr_allocator_storage{};

static constinit struct default_memory_resource : public platform::pmr::memory_resource
{
    auto do_allocate(
        std::size_t bytes,
        std::size_t alignment
    ) noexcept -> void* override
    {
        return m_upstream->allocate(bytes, alignment);
    }

    auto do_deallocate(
        void*       pointer,
        std::size_t bytes,
        std::size_t alignment
    ) noexcept -> void override
    {
        m_upstream->deallocate(pointer, alignment);
    }

    bool do_is_equal(const platform::pmr::memory_resource& other) const noexcept override
    {
        return true;
    }

    auto set_upstream(sys::heap_allocator* upstream)
    {
        m_upstream = upstream;
    }

    private:

    sys::heap_allocator* m_upstream{};

} s_default_memory_resource{};

} // namespace

bool sys::hal::board::init()
{
    using namespace sys::dev;

    // Initialize Reset and Clock Control peripheral.

    hal::peripheral_type<"/chip/rcc">::init();

    // Initialize system clock.

    clock::init();

    // Initialize the Interrupt Controller.

    arch::nvic::init();

    // Initialize UART console. TODO: get rid of the hardcoded peripheral type.

    device_type<"/dev/serial@1">::init();

    // Print Main and Process stack pointer addresses.

    log::info() << "    Main stack pointer: " << log::format::hex << log::format::showbase << arch::main_stack_pointer()    << log::endl;
    log::info() << " Process stack pointer: " << log::format::hex << log::format::showbase << arch::process_stack_pointer() << log::endl;

    hal::peripheral_type<"/chip/cortex-m-systick">    ::init();
    hal::peripheral_type<"/chip/system-configuration">::init();
    hal::peripheral_type<"/chip/exti">                ::init();
    hal::peripheral_type<"/chip/rtc">                 ::init();
    hal::peripheral_type<"/chip/sdmmc@1">             ::init();
    hal::peripheral_type<"/chip/usb-otg">             ::init();

    return true;
}

auto platform::memory::default_resource() noexcept -> platform::pmr::memory_resource*
{
    return std::addressof(s_default_memory_resource);
}

auto platform::memory::initialize_free_store(
    std::byte* heap_address_start,
    std::byte* heap_address_end
) noexcept -> bool
{
    if ( heap_address_start == nullptr || heap_address_end == nullptr )
    {
        return false;
    }

    new (s_pmr_allocator_storage.data()) sys::heap_allocator { heap_address_start, heap_address_end };

    s_default_memory_resource.set_upstream(
        reinterpret_cast<sys::heap_allocator*> (&s_pmr_allocator_storage)
    );

    return true;
}

auto platform::chrono::high_resolution_clock::now() noexcept -> high_resolution_clock::time_point
{
    const auto elapsed_tick = sys::hal::peripheral_type<"/chip/cortex-m-systick">::now();
    const auto elapsed_ms   = high_resolution_clock::time_point(std::chrono::milliseconds(elapsed_tick));
    const auto elapsed_us   = []() -> std::chrono::microseconds
    {
        return std::chrono::microseconds{ sys::hal::peripheral_type<"/chip/cortex-m-dwt">::GetCycleCounterValue() / (SystemCoreClock / 1'000'000) };
    }();

    return time_point_cast<high_resolution_clock::duration>(elapsed_ms + elapsed_us);
}

auto platform::chrono::system_clock::now() noexcept -> system_clock::time_point
{
    using namespace sys::hal;

    return {}; // TODO
}

auto platform::stdio::write(std::span<const std::byte> buffer) noexcept -> std::size_t
{
    using namespace sys::dev;

    auto n = device_type<"/dev/serial@1">::write(buffer);
    debug_assert(n == buffer.size_bytes());

    return n;
}

auto platform::stdio::read(std::span<std::byte> buffer) noexcept -> std::size_t
{
    using namespace sys::dev;

    return device_type<"/dev/serial@1">::read(buffer);
}

// HAL-overrides.

extern "C" HAL_StatusTypeDef HAL_InitTick(std::uint32_t)
{
    return HAL_OK;
}

extern "C" std::uint32_t HAL_GetTick(void)
{
    return sys::hal::peripheral_type<"/chip/cortex-m-systick">::now();
}

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
extern "C" void assert_failed(
    const char*   file,
    std::uint32_t line
)
{
    sys::log::error() << "Assertion failed in file " << file << " at line: " << line << sys::log::endl;
    sys::arch::break_point<255>();
}
