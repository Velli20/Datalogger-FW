function(set_compiler_options_for_firmware)
    set(options        "")
    set(oneValueArgs   TARGET STRICT)
    set(multiValueArgs "")

    cmake_parse_arguments(OPT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # CPU options.

    target_compile_options(${OPT_TARGET}
            PRIVATE

            # Cortex-M4F CPU options...

            # $<$<STREQUAL:$<TARGET_PROPERTY:CPU_OPTION>,Cortex-M4F>:

                # ... GCC C, C++ or Assembler.

                $<$<AND:$<COMPILE_LANGUAGE:C,CXX,ASM>,$<C_COMPILER_ID:GNU>>:
                    -mcpu=cortex-m4
                    -mfpu=fpv4-sp-d16
                    -mfloat-abi=hard
                    -mthumb
                >
            # >
    )

    # Configuration dependant C and C++ language flags.

    target_compile_options(${OPT_TARGET}
        PRIVATE
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<C_COMPILER_ID:GNU>>:

                # Debug configuration.

                $<$<CONFIG:Debug>:
                    -g3
                    -ggdb
                    -ggdb3
                    -Og
                >

                # Release configuration with minimimal size.

                $<$<CONFIG:MinSizeRel>:
                    -Os
                >

                # Release configuration with best performance.

                $<$<CONFIG:Release>:
                    -O2
                >

                # Link Time Optimization.

                $<$<CONFIG:Release>:
                    -flto
                >
            >
    )

    target_link_options(${OPT_TARGET}
        PRIVATE
            $<$<LINK_LANG_AND_ID:CXX,GNU>:
                $<$<CONFIG:Release,Debug,MinSizeRel>:

                    -mcpu=cortex-m4
                    -mfpu=fpv4-sp-d16
                    -mfloat-abi=hard
                    -mthumb

                    --specs=nano.specs   # Use Newlib Nano.
                    --specs=nosys.specs  # No additional C library features.

                    -static-libstdc++
                    -nostartfiles

                    -Wl,--gc-sections
                    -Wl,--print-memory-usage
                >

                # Link Time Optimization.

                $<$<CONFIG:Release>:-flto>
            >
    )

    # Warnings options.

    target_compile_options(${OPT_TARGET}
        PRIVATE
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<C_COMPILER_ID:GNU>>:
                $<$<BOOL:${OPT_STRICT}>:
                    -Wall
                    -Wextra
                    -Werror
                    -Wfatal-errors
                    -Wmain
                    -Wundef
                    -Wsign-conversion
                    -Wunused-parameter
                    -Wuninitialized
                    -Wmissing-declarations
                    -Wshadow
                    -Wpointer-arith
                    -Wunreachable-code
                    -Wswitch-default
                    -Wswitch-enum
                    -Wmissing-include-dirs
                    -Winit-self
                    -Wfloat-equal
                    -Wdouble-promotion
                    -Wvector-operation-performance
                    -Wduplicated-cond
                    -Wcast-align=strict
                    -Wunused-but-set-variable
                    -Wunused-but-set-parameter
                    -Wshift-count-negative
                    -Wshift-count-overflow
                    -Wshift-negative-value
                    -Woverlength-strings
                    -Wmaybe-uninitialized
                    -Wmemset-elt-size
                    -Wmemset-transposed-args
                >
            >

            # GCC: C language only.

            $<$<COMPILE_LANG_AND_ID:C,GNU>:
                $<$<BOOL:${OPT_STRICT}>:
                    -Wimplicit
                    -Wbad-function-cast
                    -Wnested-externs
                    #-Wstrict-prototypes
                >
            >

            # GCC: C++ language only.

            $<$<COMPILE_LANG_AND_ID:CXX,GNU>:
                $<$<BOOL:${OPT_STRICT}>:
                    -Woverloaded-virtual
                    -Wsuggest-override

                    # $<$<NOT:$<VERSION_LESS:CXX_COMPILER_VERSION,5.0>>:
                    #     -Wuseless-cast
                    # >
                    -Wzero-as-null-pointer-constant
                    -Wvirtual-move-assign
                    -Wstrict-null-sentinel
                    -Wno-volatile
                    # -Wold-style-cast
                    # -Wold-style-declaration
                    # -Wold-style-definition
                    # -Walloc-size-larger-than=0
                >
            >

            # GCC: Disabled warnings.

            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<C_COMPILER_ID:GNU>>:
                -Wno-switch-default
                -Wno-cast-align
                -Wno-missing-declarations
                -Wno-implicit-fallthrough
                -Wno-unused-parameter
                -Wno-switch-enum
                -Wno-psabi
                $<$<NOT:$<BOOL:${OPT_STRICT}>>:
                    -Wno-sign-conversion
                >
            >
    )

    # C and C++ language options.

    target_compile_options(${OPT_TARGET} PRIVATE

        # GCC: C language only.

        $<$<COMPILE_LANG_AND_ID:C,GNU>:
            -std=gnu17
        >

        # GCC: C and C++ language options.

        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<C_COMPILER_ID:GNU>>:
            -gdwarf-2
            $<$<BOOL:${CONFIG_FIRMWARE_STATIC_ANALYSIS}>:-fanalyzer>
            -fno-exceptions
            -fno-unwind-tables
            -ffunction-sections           # Place functions in separate section.
            -fdata-sections               # Place data in seperate sections
            -fno-common                   # Place uninitialized variables in .bss section.
            -fdevirtualize                # Attempt to convert calls to virtual functions to direct calls
            -fno-math-errno
            -fomit-frame-pointer
            -fno-strict-aliasing

            # -ftree-vectorize
            # -fsingle-precision-constant
            # -fstrict-volatile-bitfields

            # -mno-unaligned-access
            # -mno-long-calls
        >
        -fno-builtin-printf

        # GCC: C++ language only options.

        $<$<COMPILE_LANG_AND_ID:CXX,GNU>:
            -std=gnu++20
            -fconcepts
            -ftemplate-depth=128
            -fno-use-cxa-atexit
            -fno-rtti
            -fno-threadsafe-statics
            -ftemplate-backtrace-limit=0
            -fno-enforce-eh-specs
        >

        # GCC: Assembler options.

        $<$<COMPILE_LANG_AND_ID:ASM,GNU>:
            -x
            assembler-with-cpp
        >
    )

endfunction(set_compiler_options_for_firmware)
