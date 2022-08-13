cmake_minimum_required(VERSION 3.15)

set(CMAKE_CONFIGURATION_TYPES
    MinSizeRel
    Release
    Debug
)

# Generate compile_commands.json

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(Datalogger
    VERSION
        0.0.1
    DESCRIPTION
        "Datalogger-FW"
    HOMEPAGE_URL
        "https://github.com/velli20/Datalogger-FW"
    LANGUAGES
        C CXX ASM
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CMake module path.

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Modules to include.

include(CMakeParseArguments)
include(CheckCXXSourceCompiles)
include(precompiled_header)
include(std_polyfill)

# Configuration types.

message(STATUS "Available configuration types ${CMAKE_CONFIGURATION_TYPES}")

if ( CMAKE_CROSSCOMPILING )

    option(CONFIG_FIRMWARE_STATIC_ANALYSIS "Enable code static analysis"     OFF)
    option(CONFIG_SYSTEM_VIEW              "Enable Segger SystemView traces" OFF)
    option(CONFIG_TINY_USB                 "Use TinyUSB stack"                ON)
    option(CONFIG_FREERTOS                 "Use FreeRTOS"                     ON)
    option(CONFIG_CHAN_FATFS               "Use Chan FatFS"                   ON)
    option(CONFIG_LITTLEFS                 "Use LittleFS"                     ON)

endif()

# Default configuration for firmware target.

if ( NOT CMAKE_BUILD_TYPE AND CMAKE_CROSSCOMPILING )
    set(CMAKE_BUILD_TYPE MinSizeRel)
endif()

# Default configuration for test target.

if ( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CROSSCOMPILING )
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type ${CMAKE_BUILD_TYPE}")

# Set the install location to the source location if not currently specified.

if ( "${CMAKE_INSTALL_PREFIX}" STREQUAL "" OR CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
endif()

# Use folder structure for arranging files within IDEs

# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add firmware sources directory.

if ( CMAKE_CROSSCOMPILING )
    add_subdirectory(src)
endif()

# Add sub-directories. Call enable_testing() must appear before any subdirectories are added.

if ( NOT CMAKE_CROSSCOMPILING )
    enable_testing()
    add_subdirectory(tests)
endif()

