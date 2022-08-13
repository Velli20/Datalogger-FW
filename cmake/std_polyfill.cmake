
set(CXXSTD_CONSTRUCT_AT_TEST_SOURCE
[[

#include <memory>

struct S
{
    S(int v)
        : m_val{v}
    {}

    int m_val{};
};

int main()
{
    alignas(S) unsigned char storage[sizeof(S)];

    S* ptr = std::construct_at(reinterpret_cast<S*>(storage), 42);

    std::destroy_at(ptr);
}

]])

macro(std_polyfill_test TARGET)

    check_cxx_source_compiles("${CXXSTD_CONSTRUCT_AT_TEST_SOURCE}" CPP20_HAS_CONSTRUCT_AT)

    target_compile_definitions(${TARGET}
        PUBLIC
            $<$<BOOL:${CPP20_HAS_CONSTRUCT_AT}>:cmake_compile_test_std_construct_at>
    )

endmacro()
