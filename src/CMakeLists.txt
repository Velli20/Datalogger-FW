include(set_compiler_options_for_firmware)
include(preprocess_linker_script)
include(JLinkFlash)

# Configure firmware configuration header.

configure_file(${PROJECT_SOURCE_DIR}/src/firmware_config.hpp.in firmware_config.hpp @ONLY)

# "Expected" library.

add_library(libexpected INTERFACE)

target_include_directories(libexpected
    INTERFACE
        ${PROJECT_SOURCE_DIR}/external/expected/include/tl
)

# ARM CMSIS library.

add_library(libcmsis
    EXCLUDE_FROM_ALL
        ${PROJECT_SOURCE_DIR}/external/cmsis_device_l4/Source/Templates/system_stm32l4xx.c
)

target_include_directories(libcmsis
    PUBLIC
        ${PROJECT_SOURCE_DIR}/external/cmsis_device_l4/Include
        ${PROJECT_SOURCE_DIR}/external/cmsis_core/Include
)

target_compile_definitions(libcmsis
    PUBLIC
        $<$<COMPILE_LANGUAGE:CXX,C,ASM>:
            STM32L4S5xx
        >
)

set_compiler_options_for_firmware(
    TARGET
        libcmsis
    STRICT
        OFF
)

# STMicroelectronics STM32L4-HAL library.

add_library(libstm32l4
    EXCLUDE_FROM_ALL
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_i2c.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_i2c_ex.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_rcc.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_rcc_ex.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_sd.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_sd_ex.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_ospi.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_pcd.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_pcd_ex.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_pwr.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_pwr_ex.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_dma.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_hal_gpio.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_ll_usb.c
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Src/stm32l4xx_ll_sdmmc.c
)

target_include_directories(libstm32l4
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/L4-hal/stm32l4
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Inc
    PRIVATE
        ${PROJECT_SOURCE_DIR}/external/stm32l4xx_hal_driver/Inc/Legacy
)

target_link_libraries(libstm32l4
    PRIVATE
        libcmsis
)

set_compiler_options_for_firmware(
    TARGET
        libstm32l4
    STRICT
        OFF
)

target_compile_definitions(libstm32l4
    PUBLIC
        $<$<COMPILE_LANGUAGE:CXX,C,ASM>:
            USE_HAL_DRIVER
            USE_HAL_I2C_REGISTER_CALLBACKS=0
            USE_HAL_USART_REGISTER_CALLBACKS=0
            USE_HAL_UART_REGISTER_CALLBACKS=0
            USE_HAL_SPI_REGISTER_CALLBACKS=0
            USE_HAL_RTC_REGISTER_CALLBACKS=0
            USE_HAL_LPTIM_REGISTER_CALLBACKS=0
            USE_HAL_PCD_REGISTER_CALLBACKS=0
            USE_FULL_LL_DRIVER

            $<$<CONFIG:Debug>:
                USE_FULL_ASSERT
            >
        >
)

# FreeRTOS library.

add_library(libfreertos
    EXCLUDE_FROM_ALL
        ${PROJECT_SOURCE_DIR}/external/FreeRTOS-Kernel/tasks.c
        ${PROJECT_SOURCE_DIR}/external/FreeRTOS-Kernel/queue.c
        ${PROJECT_SOURCE_DIR}/external/FreeRTOS-Kernel/list.c

        $<$<C_COMPILER_ID:GNU>:
            ${PROJECT_SOURCE_DIR}/external/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c
        >
)

target_include_directories(libfreertos
    PUBLIC
        ${PROJECT_SOURCE_DIR}/external/FreeRTOS-Kernel
        ${PROJECT_SOURCE_DIR}/external/FreeRTOS-Kernel/include
        ${PROJECT_SOURCE_DIR}/external/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
        ${PROJECT_SOURCE_DIR}/src/L9-porting/freertos
)

target_link_libraries(libfreertos
    PRIVATE
        libcmsis
)

set_compiler_options_for_firmware(
    TARGET
        libfreertos
    STRICT
        OFF
)

# TinyUSB library.

add_library(libtinyusb
    EXCLUDE_FROM_ALL
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/tusb.c
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/class/msc/msc_device.c
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/class/cdc/cdc_device.c
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/common/tusb_fifo.c
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/device/usbd.c
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/device/usbd_control.c
)

target_compile_definitions(libtinyusb
    PUBLIC
        CFG_TUSB_MCU=OPT_MCU_STM32L4
        CFG_TUSB_OS=OPT_OS_FREERTOS
        CFG_TUSB_MODE=OPT_MODE_DEVICE
)

target_include_directories(libtinyusb
    PUBLIC
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/class/msc
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/class/cdc
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/osal
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/common
        ${PROJECT_SOURCE_DIR}/external/tinyusb/src/device
        ${PROJECT_SOURCE_DIR}/src/L9-porting/tinyusb
)

target_link_libraries(libtinyusb
    PRIVATE
        libcmsis
        libfreertos
)

set_compiler_options_for_firmware(
    TARGET
        libtinyusb
    STRICT
        OFF
)

# LittleFs library.

add_library(liblittlefs
    EXCLUDE_FROM_ALL
        ${PROJECT_SOURCE_DIR}/external/littlefs/lfs.c
        ${PROJECT_SOURCE_DIR}/external/littlefs/lfs_util.c
        ${PROJECT_SOURCE_DIR}/external/littlefs/bd/lfs_filebd.c
)

target_compile_definitions(liblittlefs
    PUBLIC
        LFS_CONFIG=littlefs_config.h
        $<$<CONFIG:Debug>:
            LFS_TRACE_ENABLED
        >
)

target_include_directories(liblittlefs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/external/littlefs
        ${PROJECT_SOURCE_DIR}/external/littlefs/bd
        ${PROJECT_SOURCE_DIR}/src/L9-porting/littlefs
)

target_link_libraries(liblittlefs
    PRIVATE
        libcmsis
)

set_compiler_options_for_firmware(
    TARGET
        liblittlefs
    STRICT
        OFF
)

# Chan FatFS library.

add_library(libfatfs
    EXCLUDE_FROM_ALL
        ${PROJECT_SOURCE_DIR}/external/fatfs/option/ccsbcs.c
        ${PROJECT_SOURCE_DIR}/external/fatfs/ff.c
)

target_include_directories(libfatfs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/external/fatfs
        ${PROJECT_SOURCE_DIR}/src/L9-porting/chan_fatfs
)

set_compiler_options_for_firmware(
    TARGET
        libfatfs
    STRICT
        OFF
)

# SEGGER SystemView library.

add_library(libsystemview
    EXCLUDE_FROM_ALL
        ${PROJECT_SOURCE_DIR}/external/SystemView/SEGGER/SEGGER_RTT.c
        ${PROJECT_SOURCE_DIR}/external/SystemView/SEGGER/SEGGER_SYSVIEW.c
        $<$<C_COMPILER_ID:GNU>:
            ${PROJECT_SOURCE_DIR}/external/SystemView/SEGGER/SEGGER_RTT_ASM_ARMv7M.s
        >
)

target_include_directories(libsystemview
    PUBLIC
        ${PROJECT_SOURCE_DIR}/external/SystemView/SEGGER
        ${PROJECT_SOURCE_DIR}/external/SystemView/Config
)

target_link_libraries(libsystemview
    PRIVATE
        libcmsis
        $<$<BOOL:${CONFIG_FREERTOS}>:libfreertos>
)

set_compiler_options_for_firmware(
    TARGET
        libsystemview
    STRICT
        OFF
)

# Main firmware target.

add_executable(firmware

    $<$<BOOL:${CONFIG_FREERTOS}>:
        ${PROJECT_SOURCE_DIR}/src/L9-porting/freertos/port.cc
    >

    $<$<AND:$<BOOL:${CONFIG_SYSTEM_VIEW}>,$<BOOL:${CONFIG_FREERTOS}>>:
        ${PROJECT_SOURCE_DIR}/src/L2-operating-system/systemview/SEGGER_SYSVIEW_Config_FreeRTOS.c
        ${PROJECT_SOURCE_DIR}/src/L2-operating-system/systemview/SEGGER_SYSVIEW_FreeRTOS.c
    >

    $<$<BOOL:${CONFIG_TINY_USB}>:
        ${PROJECT_SOURCE_DIR}/src/L9-porting/tinyusb/tu_device.cc
        ${PROJECT_SOURCE_DIR}/src/L9-porting/tinyusb/tu_msc.cc
        ${PROJECT_SOURCE_DIR}/src/L4-hal/stm32l4/usb_dcd.cc # TODO: Move to port.
    >

    $<$<BOOL:${CONFIG_CHAN_FATFS}>:
        ${PROJECT_SOURCE_DIR}/src/L9-porting/chan_fatfs/fatfs_diskio.cc
    >

    $<$<BOOL:${CONFIG_LITTLEFS}>:
        ${PROJECT_SOURCE_DIR}/src/L9-porting/littlefs/port.cc
    >

    ${PROJECT_SOURCE_DIR}/src/L1-arch/cortex-m/handle_bad_interrupt.cc
    ${PROJECT_SOURCE_DIR}/src/L1-arch/cortex-m/cortex_m_startup.cc
    ${PROJECT_SOURCE_DIR}/src/L4-hal/stm32l4/board_impl.cc
    ${PROJECT_SOURCE_DIR}/src/L7-runtime/globals.cc
    ${PROJECT_SOURCE_DIR}/src/L8-main-process/libc/assert.cc
    ${PROJECT_SOURCE_DIR}/src/L8-main-process/libc/retarget.cc
    ${PROJECT_SOURCE_DIR}/src/L8-main-process/libc/syscalls.cc
    ${PROJECT_SOURCE_DIR}/src/L8-main-process/libcxx/itanium_cxxabi.cc
    ${PROJECT_SOURCE_DIR}/src/L8-main-process/libcxx/new.cc
    ${PROJECT_SOURCE_DIR}/src/L8-main-process/main_process.cc
)

target_include_directories(firmware
    PRIVATE
        ${PROJECT_SOURCE_DIR}/external
        ${PROJECT_SOURCE_DIR}/src/L0-base
        ${PROJECT_SOURCE_DIR}/src/L1-arch
        ${PROJECT_SOURCE_DIR}/src/L2-operating-system
        ${PROJECT_SOURCE_DIR}/src/L3-configuration
        ${PROJECT_SOURCE_DIR}/src/L4-hal
        ${PROJECT_SOURCE_DIR}/src/L5-devices
        ${PROJECT_SOURCE_DIR}/src/L5-devices/graphics
        ${PROJECT_SOURCE_DIR}/src/L6-filesystem
        ${PROJECT_SOURCE_DIR}/src/L7-runtime
        ${PROJECT_SOURCE_DIR}/src/L8-main-process
        ${CMAKE_CURRENT_BINARY_DIR}

        $<$<BOOL:${CONFIG_SYSTEM_VIEW}>:
            ${PROJECT_SOURCE_DIR}/src/L2-operating-system/systemview
        >
)

target_link_options(firmware
    PRIVATE
        -Xlinker
        -Map=$<TARGET_FILE:firmware>.map
)

preprocess_linker_script(
    TARGET
        firmware
    LINKER_SCRIPT_TEMPLATE
        ${PROJECT_SOURCE_DIR}/src/L3-configuration/linker_script.ld.inc
)

# set_property(TARGET firmware
#     PROPERTY
#         LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT}
# )

target_compile_definitions(firmware
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX,C,ASM>:
            # STM32L4S5xx
            # USE_HAL_DRIVER

            # $<$<CONFIG:Debug>:
            #     USE_FULL_ASSERT
            # >
        >

        $<$<BOOL:${CONFIG_SYSTEM_VIEW}>:
            CONFIG_USE_SEGGER_SYSTEM_VIEW
            SEGGER_RTT_SECTION=".segger_rtt"
        >
)

target_link_libraries(firmware
    PRIVATE
        libcmsis
        libstm32l4
        libexpected
        $<$<BOOL:${CONFIG_FREERTOS}>:libfreertos>
        $<$<BOOL:${CONFIG_TINY_USB}>:libtinyusb>
        $<$<BOOL:${CONFIG_LITTLEFS}>:liblittlefs>
        $<$<BOOL:${CONFIG_CHAN_FATFS}>:libfatfs>
        $<$<BOOL:${CONFIG_SYSTEM_VIEW}>:libsystemview>
)

# Set compiler options.

set_compiler_options_for_firmware(
    TARGET
        firmware
    STRICT
        ON
)

# Standard library compilation test.

std_polyfill_test(firmware)

# Standard library headers and polyfills are included in the precompiled header.

precompiled_header(firmware)

set_target_properties(firmware PROPERTIES
    OUTPUT_NAME
        firmware
    SUFFIX
        ".elf"
    CPU_OPTION
        "Cortex-M4F"
    LINKER_LANGUAGE
        CXX
)

# JLink flash target for the final firmware image.

JLinkFlash(
    TARGET
        firmware
    DEVICE
        STM32L4S5VI
    HEX_EXECUTABLE
        ${CMAKE_CURRENT_BINARY_DIR}/firmware.hex
    ELF_EXECUTABLE
        ${CMAKE_CURRENT_BINARY_DIR}/firmware.elf
)

# Post-build commands: Generate *.bin and *.hex file.

add_custom_command(POST_BUILD
    TARGET
        firmware
    COMMAND
        ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:firmware> $<TARGET_PROPERTY:firmware,NAME>.hex
    COMMAND
        ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:firmware> $<TARGET_PROPERTY:firmware,NAME>.bin
    DEPENDS
        $<TARGET_FILE:firmware>
    COMMENT
        "Generating .hex and .bin file..."
)

