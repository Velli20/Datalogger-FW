set(CMAKE_SYSTEM_NAME      Custom)
set(CMAKE_SYSTEM_VERSION   1)
set(CMAKE_SYSTEM_PROCESSOR arm)

# The cross-compiler
# if(COMPILE_ON_ARM)
# 	if("${COMPILER_VERSION}" STREQUAL "10.3-2021.10")
# 		set(GCC_ARM_NONE_EABI_DOWNLOAD_URL https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-aarch64-linux.tar.bz2)
# 		set(GCC_ARM_NONE_EABI_MD5 3fe3d8bb693bd0a6e4615b6569443d0d)
# 	else()
# 		message(FATAL_ERROR "Unknown compiler version: ${COMPILER_VERSION} for ${CMAKE_HOST_SYSTEM_PROCESSOR}")
# 	endif()
# else()
# 	if("${COMPILER_VERSION}" STREQUAL "8-2019-q3-update")
# 		set(GCC_ARM_NONE_EABI_DOWNLOAD_URL https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2019q3/RC1.1/gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2?revision=c34d758a-be0c-476e-a2de-af8c6e16a8a2?product=GNU%20Arm%20Embedded%20Toolchain,64-bit,,Linux,8-2019-q3-update)
# 		set(GCC_ARM_NONE_EABI_MD5 6341f11972dac8de185646d0fbd73bfc)
# 	elseif("${COMPILER_VERSION}" STREQUAL "10.3-2021.10")
# 		set(GCC_ARM_NONE_EABI_DOWNLOAD_URL https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2)
# 		set(GCC_ARM_NONE_EABI_MD5 2383e4eb4ea23f248d33adc70dc3227e)
# 	else()
# 		message(FATAL_ERROR "Unknown compiler version: ${COMPILER_VERSION}")
# 	endif()
# endif()

# ExternalProject_Add(gcc_arm_none_eabi
# 	DOWNLOAD_DIR ${WORKSPACE_DIR}/downloads
# 	URL ${GCC_ARM_NONE_EABI_DOWNLOAD_URL}
# 	URL_MD5 ${GCC_ARM_NONE_EABI_MD5}
# 	SOURCE_DIR ${WORKSPACE_DIR}/tools/gcc_arm_none_eabi
# 	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo "Skipping configure step."
# 	BUILD_COMMAND ${CMAKE_COMMAND} -E echo "Skipping build step."
# 	INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."
# 	)

# If toolchain path variable is not set then try locate one in commonly known directories.

if ( NOT ARM_TOOLCHAIN_DIR )

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Windows )
        set(ARM_TOOLCHAIN_DIR "C:\\Program Files (x86)\\Arm GNU Toolchain arm-none-eabi\\11.2 2022.02")
    endif()

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Linux )
        set(ARM_TOOLCHAIN_DIR "/usr/bin")
    endif()

    if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin )
        set(ARM_TOOLCHAIN_DIR "/Applications/ARM")
    endif()

    message(INFO "Variable Variable ARM_TOOLCHAIN_DIR=(${ARM_TOOLCHAIN_DIR}) is undefined")

endif()

# Set executable extension.

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# Convert toolchain directory path separators on Windows platform.

if ( CMAKE_HOST_SYSTEM_NAME STREQUAL Windows )
    string(REPLACE "\\" "/" ARM_TOOLCHAIN_DIR ${ARM_TOOLCHAIN_DIR})
endif()

unset(CMAKE_C_COMPILER)
unset(CMAKE_CXX_COMPILER)
unset(CMAKE_ASM_COMPILER)
unset(CMAKE_OBJCOPY)
unset(CMAKE_OBJDUMP)
unset(CMAKE_SIZE)

# Find required toolchain programs.

find_program(CMAKE_C_COMPILER   NAMES arm-none-eabi-gcc     PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_CXX_COMPILER NAMES arm-none-eabi-g++     PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_ASM_COMPILER NAMES arm-none-eabi-gcc     PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_OBJCOPY      NAMES arm-none-eabi-objcopy PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_OBJDUMP      NAMES arm-none-eabi-objdump PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_SIZE         NAMES arm-none-eabi-size    PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_GDB          NAMES arm-none-eabi-gdb     PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_NM           NAMES arm-none-eabi-nm      PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_CPP_FILTER   NAMES arm-none-eabi-c++filt PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")
find_program(CMAKE_READELF      NAMES arm-none-eabi-readelf PATHS ${ARM_TOOLCHAIN_DIR} PATH_SUFFIXES "/bin")

if ( ${CMAKE_C_COMPILER} MATCHES CMAKE_C_COMPILER-NOTFOUND )
    message(FATAL_ERROR "Cannot find ARM-GCC toolchain program \"arm-none-eabi-gcc\". Variable ARM_TOOLCHAIN_DIR=(${ARM_TOOLCHAIN_DIR})")
endif()

if ( ${CMAKE_C_COMPILER} MATCHES CMAKE_CXX_COMPILER-NOTFOUND )
    message(FATAL_ERROR "Cannot find ARM-GCC toolchain program \"arm-none-eabi-g++\". Variable ARM_TOOLCHAIN_DIR=(${ARM_TOOLCHAIN_DIR})")
endif()

message(STATUS "CMAKE_C_COMPILER=\"${CMAKE_C_COMPILER}\"")

set(CMAKE_C_COMPILER_FORCED   TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# Clear any default compiler flags set by CMake.

unset(CMAKE_C_FLAGS          CACHE)
unset(CMAKE_CXX_FLAGS        CACHE)
unset(CMAKE_ASM_FLAGS        CACHE)
unset(CMAKE_EXE_LINKER_FLAGS CACHE)

set(CMAKE_C_FLAGS          "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS        "" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS        "" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "" CACHE STRING "" FORCE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")         # +s, flag for exe link to use shared lib
set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG "")       # -rpath
set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG_SEP "")   # : or empty


set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS   "-shared -nostartfiles")
set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "-shared -nostartfiles")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")

set(CMAKE_SHARED_LIBRARY_C_FLAGS   "-fPIC")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "-fPIC")

set(CMAKE_CXX_CREATE_SHARED_LIBRARY
      "<CMAKE_CXX_COMPILER> <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")

set(CMAKE_C_CREATE_SHARED_LIBRARY
      "<CMAKE_C_COMPILER> <CMAKE_SHARED_LIBRARY_C_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")


# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

