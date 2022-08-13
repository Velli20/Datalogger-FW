// Includes.

#include <base.hpp>
#include <hal.hpp>

namespace sys::newlib {

extern "C" void* malloc(std::size_t size)
{
    void* ptr = arch::service_call<os::service_call_code_type::k_allocate_memory>(size);
    debug_assert(ptr != nullptr);

    return ptr;
}

extern "C" void* calloc(std::size_t count, std::size_t size)
{
    void* ptr = arch::service_call<os::service_call_code_type::k_allocate_memory>(size * count);
    debug_assert(ptr != nullptr);

    std::memset(ptr, 0, size * count);

    return ptr;
}

extern "C" void free(void* ptr)
{
    const bool res = arch::service_call<os::service_call_code_type::k_free_memory>(ptr);
    debug_assert(res == true);
}

extern "C" char* strerror(int /* errnum */)
{
    return nullptr;
}

extern "C" int getchar(void)
{
    return EOF;
}

extern "C" int putchar([[maybe_unused]] int c)
{
    auto out   = std::span{std::addressof(c), 1};
    auto count = platform::stdio::write(std::as_bytes(out));

    return static_cast<int>(count);
}

extern "C" int puts([[maybe_unused]] const char* data)
{
    auto out   = std::span{data, strnlen(data, 1024)};
    auto count = platform::stdio::write(std::as_bytes(out));

    return static_cast<int>(count);
}

extern "C" std::FILE* fopen(const char* p, const char* m)
{
    return nullptr;
}

extern "C" int fclose(std::FILE*)
{
    return EOF;
}

extern "C" std::size_t fread(void*, std::size_t, std::size_t, std::FILE*)
{
    return 0;
}

extern "C" std::size_t fwrite(const void*, std::size_t, std::size_t, std::FILE*)
{
    return 0;
}

extern "C" long ftell(std::FILE*)
{
    return 0;
}

extern "C" int fseek(std::FILE*, long, int)
{
    return EOF;
}

extern "C" char* fgets(char*, int, FILE*)
{
    return nullptr;
}

extern "C" int fputs(const char*, std::FILE*)
{
    return EOF;
}

extern "C" int fputc(int, std::FILE*)
{
    return EOF;
}

extern "C" int feof(std::FILE*)
{
    return EOF;
}

} // namespace sys::newlib
