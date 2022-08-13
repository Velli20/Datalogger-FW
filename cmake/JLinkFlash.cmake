function(JLinkFlash)
    set(options        "")
    set(oneValueArgs   TARGET DEVICE HEX_EXECUTABLE ELF_EXECUTABLE GDB_SERVER_PORT)
    set(multiValueArgs "")

    # if(DOWNLOAD_JLINK)

    #         # The JLink tool
    #     set(JLINK_DOWNLOAD_URL https://www.segger.com/downloads/jlink)
    #     if(COMPILE_ON_ARM)
    #     	message(STATUS "Get ARM (64-bit) version of JLink tool")
    #     	set(JLINK_DEB_FILE JLink_Linux_V758_arm64.deb)
    #     	set(JLINK_MD5 f6a74b6a134fed8036238b107504369b)
    #     else()
    #     	set(JLINK_DEB_FILE JLink_Linux_V754b_x86_64.deb)
    #     	set(JLINK_MD5 999c69c040168cffb272850fc62a3708)
    #     endif()

	#     set(JLINK_TARGET jlink)
	#     message(STATUS "Configure ${JLINK_TARGET} target")
	#     ExternalProject_Add(${JLINK_TARGET}
	#     	DOWNLOAD_COMMAND wget --timestamping --post-data "accept_license_agreement=accepted&non_emb_ctr=confirmed" ${JLINK_LINUX_DOWNLOAD_URL}
	#     	DOWNLOAD_DIR ${WORKSPACE_DIR}/downloads
	#     	URL ${JLINK_LINUX_DOWNLOAD_URL}
	#     	URL_MD5 ${JLINK_MD5}
	#     	DOWNLOAD_NO_EXTRACT 1
	#     	SOURCE_DIR ${WORKSPACE_DIR}/tools/jlink
	#     	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo "Skipping configure step."
	#     	BUILD_COMMAND ${CMAKE_COMMAND} -E echo "Skipping build step."
	#     	INSTALL_COMMAND ${SUPERUSER_SWITCH} dpkg -i ${WORKSPACE_DIR}/downloads/${JLINK_DEB_FILE}
	#     	# USES_TERMINAL_INSTALL 1 # Does not actually work
	#     	)
    # endif()

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Set default JLink program path.

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin OR CMAKE_HOST_SYSTEM_NAME STREQUAL Linux )
        list(APPEND Jlink_SearchPaths "/usr/local/bin")
    endif()

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Windows )
        list(APPEND Jlink_SearchPaths "C:\\Program Files\\SEGGER\\JLink")
    endif()

    # Check arguments.

    if ( NOT DEFINED ARG_DEVICE )
        message(FATAL_ERROR "DEVICE is undefined")
    endif()

    if ( NOT DEFINED ARG_HEX_EXECUTABLE )
        message(FATAL_ERROR "HEX_EXECUTABLE is undefined")
    endif()

    if ( NOT DEFINED ARG_ELF_EXECUTABLE )
        message(FATAL_ERROR "ELF_EXECUTABLE is undefined")
    endif()

    # Set GDB server port to default.

    if ( NOT DEFINED ARG_GDB_SERVER_PORT OR ARG_GDB_SERVER_PORT STREQUAL "" )
        set(ARG_GDB_SERVER_PORT 2331)
    endif()

    # Find JLink programs.

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin OR CMAKE_HOST_SYSTEM_NAME STREQUAL Linux )
        find_program(JLinkExe_Program NAMES "JLinkExe" PATHS ${Jlink_SearchPaths})
    endif()

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Windows )
        find_program(JLinkExe_Program NAMES "JLink" PATHS ${Jlink_SearchPaths})
    endif()

    find_program(JLinkGDBServer_Program NAMES "JLinkGDBServerCL" PATHS ${Jlink_SearchPaths})
    find_program(JLinkSWOViewr_Program  NAMES "JLinkSWOViewerCL" PATHS ${Jlink_SearchPaths})

    if ( ${JLinkExe_Program} STREQUAL JLinkExe_Program-NOTFOUND OR ${JLinkGDBServer_Program} STREQUAL JLinkGDBServer_Program-NOTFOUND )
        message(FATAL_ERROR "Could not locate Jlink tools")
    endif()

    message(STATUS "JLinkExe_Program       ${JLinkExe_Program}")
    message(STATUS "JLinkGDBServer_Program ${JLinkGDBServer_Program}")
    message(STATUS "JLinkSWOViewr_Program  ${JLinkSWOViewr_Program}")

    # Configure JLink flash and GDB debug template files.

    set(JLINK_HEX_EXECUTABLE ${ARG_HEX_EXECUTABLE})
    set(JLINK_GDB_LOCALHOST  "localhost:${ARG_GDB_SERVER_PORT}")

    configure_file("${CMAKE_MODULE_PATH}/templates/JlinkGnuDebugger.txt.in"     "${CMAKE_BINARY_DIR}/debug.txt")
    configure_file("${CMAKE_MODULE_PATH}/templates/JlinkCommanderScript.txt.in" "${CMAKE_BINARY_DIR}/flash.txt")

    # Add JLink flash program arguments.

    list(APPEND JLinkExeARGS -NoGui 1)
    list(APPEND JLinkExeARGS -device ${ARG_DEVICE})
    list(APPEND JLinkExeARGS -if SWD)
    list(APPEND JLinkExeARGS -speed 4000)
    list(APPEND JLinkExeARGS -autoconnect 1)
    list(APPEND JLinkExeARGS -CommanderScript ${CMAKE_BINARY_DIR}/flash.txt)

    # Add JLink flash target.

    add_custom_target(flash COMMENT           "Programming ${ARG_HEX_EXECUTABLE}"
                            DEPENDS           ${ARG_TARGET}
                            COMMAND           ${JLinkExe_Program} ${JLinkExeARGS}
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                            USES_TERMINAL)

    macro(appendGDBServerARGS ARG)

        if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin OR CMAKE_HOST_SYSTEM_NAME STREQUAL Linux )
            set(JLinkGDBServerArgs "${JLinkGDBServerArgs} \\\n\t${ARG}")
        endif()

        if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Windows )
            set(JLinkGDBServerArgs "${JLinkGDBServerArgs}^\n\t${ARG}")
        endif()

    endmacro()

    # Add JLinkGDBServer program arguments.

    appendGDBServerARGS("-port          ${ARG_GDB_SERVER_PORT}")
    # appendGDBServerARGS("-swoport       ${ARG_SWO_PORT}")
    # appendGDBServerARGS("-telnetport    ${ARG_TELNET_PORT}")
    # appendGDBServerARGS("-RTTTelnetPort ${ARG_RTT_TELNET_PORT}")
    appendGDBServerARGS("-select USB")
    appendGDBServerARGS("-device ${ARG_DEVICE}")
    appendGDBServerARGS("-endian little")
    appendGDBServerARGS("-if SWD")
    appendGDBServerARGS("-speed 4000")
    appendGDBServerARGS("-noir")
    appendGDBServerARGS("-LocalhostOnly")
    appendGDBServerARGS("-singlerun")
    appendGDBServerARGS("-nogui")
    appendGDBServerARGS("-excdbg 10")
    appendGDBServerARGS("-rtos GDBServer/RTOSPlugin_FreeRTOS.dylib")

    # GDB program arguments.

    set(GDB_SCRIPT_PATH "${CMAKE_BINARY_DIR}/debug.txt")

    # Add debug target for UNIX platforms.

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin OR CMAKE_HOST_SYSTEM_NAME STREQUAL Linux )

        # Configure debug script.

        configure_file("${CMAKE_MODULE_PATH}/templates/JlinkDebug.sh.in" "${CMAKE_BINARY_DIR}/JlinkDebug.sh")

        add_custom_target(debug COMMENT           "Starting JLink GDB session"
                                DEPENDS           ${ARG_TARGET}
                                COMMAND           bash ${CMAKE_BINARY_DIR}/JlinkDebug.sh
                                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                                USES_TERMINAL)

    endif()

    # Add debug target for Windows platforms.

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Windows )

        # Configure debug script.

        configure_file("${CMAKE_MODULE_PATH}/templates/JlinkDebug.bat.in" "${CMAKE_BINARY_DIR}/JlinkDebug.bat")

        add_custom_target(debug COMMENT           "Starting JLink GDB session"
                                DEPENDS           ${ARG_TARGET}
                                COMMAND           cmd /c ${CMAKE_BINARY_DIR}/JlinkDebug.bat
                                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                                USES_TERMINAL)

    endif()

endfunction()
