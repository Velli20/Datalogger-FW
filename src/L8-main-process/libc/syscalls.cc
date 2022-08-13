
// Includes.

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

extern "C" void* _sbrk(std::size_t size)
{
    std::puts("_sbrk");

    // FIXME:

    return std::malloc(size);
}

extern "C" int _isatty (int)                    { return  1; }
extern "C" int _lseek  (int, int, int)          { return  0; }
extern "C" int _open   (const char*, int, int)  { return -1; }
extern "C" int _close  (int)                    { return -1; }
extern "C" int _read   (int, char*, int)        { return  0; }
extern "C" int _write  (int, char*, int)        { return  0; }
extern "C" int _fstat  (int, void*)             { return  0; }
extern "C" int _getpid (void)                   { return  1; }
extern "C" int _kill   (int, int)               { return -1; }

extern "C" void _exit(int)
{
    std::puts("_exit");

    while ( true )
    {
        ;;
    }
}

extern "C" void exit          (int s) { _exit(s); }
extern "C" void quick_exit    (int s) { _exit(s); }
extern "C" void _Exit         (int s) { _exit(s); }
extern "C" void abort         (void)  { _exit(1); }

extern "C" int  atexit        (void (*)(void)) { return 0; }
extern "C" int  at_quick_exit (void (*)(void)) { return 0; }

extern "C" void _gettimeofday([[maybe_unused]] struct timeval* tv,
                              [[maybe_unused]] void* tzvp)
{ }

