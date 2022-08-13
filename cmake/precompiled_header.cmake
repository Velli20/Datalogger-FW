macro(precompiled_header TARGET)

    target_precompile_headers(${TARGET}
        PRIVATE

            $<$<COMPILE_LANGUAGE:CXX>:<algorithm$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<array$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<atomic$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<bit$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<bitset$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<charconv$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<chrono$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<concepts$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<compare$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<functional$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<filesystem$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<fstream$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<limits$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<initializer_list$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<iterator$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<optional$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<memory$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<span$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<system_error$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<variant$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<tuple$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<type_traits$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<utility$<ANGLE-R>>

            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/L0-base/polyfill/concepts.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/L0-base/polyfill/functional.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/L0-base/polyfill/bit.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/L0-base/polyfill/memory.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/L0-base/polyfill/utility.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/L0-base/polyfill/pmr.hpp>

            $<$<COMPILE_LANGUAGE:CXX>:<cstdlib$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<cmath$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<cstddef$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<cstdint$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<ctime$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<cstring$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<cassert$<ANGLE-R>>
            $<$<COMPILE_LANGUAGE:CXX>:<cstdio$<ANGLE-R>>
    )

endmacro()