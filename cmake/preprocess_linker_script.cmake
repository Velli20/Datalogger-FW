function(preprocess_linker_script)
    set(options        "")
    set(oneValueArgs   TARGET LINKER_SCRIPT_TEMPLATE)
    set(multiValueArgs "")

    cmake_parse_arguments(OPT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if ( NOT OPT_LINKER_SCRIPT_TEMPLATE )
        message(FATAL_ERROR "No linker script template provided")
    endif()

    # Get file basename.

    get_filename_component(LINKER_SCRIPT_NAME ${OPT_LINKER_SCRIPT_TEMPLATE} NAME_WE)

    # Pre-process linker script (FIXME: -I${PROJECT_SOURCE_DIR}).

    add_custom_target(CREATE_LINKER_SCRIPT
        COMMAND
            ${CMAKE_C_COMPILER}
                -I${PROJECT_SOURCE_DIR}
                -E -x assembler-with-cpp
                -P ${OPT_LINKER_SCRIPT_TEMPLATE} -o ${CMAKE_BINARY_DIR}/${LINKER_SCRIPT_NAME}.ld
    )

    add_dependencies(${OPT_TARGET} CREATE_LINKER_SCRIPT)

    target_link_options(${OPT_TARGET} PRIVATE "-T" "${CMAKE_BINARY_DIR}/${LINKER_SCRIPT_NAME}.ld")

endfunction()