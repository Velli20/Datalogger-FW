$CMAKE            = 'C:\Program Files\CMake\bin\cmake.exe'
$NINJA            = 'C:\Ninja-build\ninja.exe'
$CMAKE_ARGS       = '-G', 'Ninja', `
                    '-D', 'CMAKE_TOOLCHAIN_FILE=cmake/Toolchain-ARM-GCC.cmake', `
                    '-D', 'ARM_TOOLCHAIN_DIR=C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\11.2 2022.02', `
                    '-D', "CMAKE_MAKE_PROGRAM=$NINJA", `
                    '-D', "SEGGER_PROGRAM_DIR=C:\Program Files\SEGGER\JLink"

&$CMAKE $LIBGLUI_REPO_DIR -B build $CMAKE_ARGS
&$CMAKE --build build -j 8 -- $args


