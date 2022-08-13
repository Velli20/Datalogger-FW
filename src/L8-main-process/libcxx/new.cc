// Includes.

#include <cstddef>
#include <cstdlib>

void* operator new(std::size_t size)
{
    size = std::max(1U, size);

    do
    {
        if ( void* mem = std::malloc(size); mem != nullptr )
        {
            return mem;
        }

        if ( const std::new_handler handler = std::get_new_handler(); handler != nullptr )
        {
            handler();
            continue;
        }
    }
    while ( false );

    std::exit(EXIT_FAILURE);

    while ( true )
    {
        ;;
    }
}

void* operator new[](std::size_t size)
{
    size = std::max(1U, size);

    do
    {
        if ( void* mem = std::malloc(size); mem != nullptr )
        {
            return mem;
        }

        if ( const std::new_handler handler = std::get_new_handler(); handler != nullptr )
        {
            handler();
            continue;
        }
    }
    while ( false );

    std::exit(EXIT_FAILURE);

    while ( true )
    {
        ;;
    }
}

void* operator new  (std::size_t size, const std::nothrow_t&) noexcept { return std::malloc(std::max(1U, size)); }
void* operator new[](std::size_t size, const std::nothrow_t&) noexcept { return std::malloc(std::max(1U, size)); }

void operator delete[](void* ptr)              noexcept { std::free(ptr); }
void operator delete[](void* ptr, std::size_t) noexcept { std::free(ptr); }
void operator delete  (void* ptr, std::size_t) noexcept { std::free(ptr); }
void operator delete  (void* ptr)              noexcept { std::free(ptr); }
