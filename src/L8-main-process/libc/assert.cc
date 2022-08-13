
#include <base.hpp>

void __assert_func(const char* file, int /* line */, const char* func, const char* expr)
{
    using namespace sys;

    log::error()
        << "assertion \""
        << expr
        << "\" failed in file \""
        << file
        << "\""
        << " in in function \""
        << func
        << "\"";

    std::exit(EXIT_FAILURE);
}

