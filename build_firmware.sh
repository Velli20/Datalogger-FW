#!/bin/sh

function main()
{
	# Build command.

    cmake . -B build                                              \
            -G "Ninja"                                            \
            -D CMAKE_TOOLCHAIN_FILE=cmake/Toolchain-ARM-GCC.cmake \
            -D ARM_TOOLCHAIN_DIR=/Applications/ARM

    (cd build && ninja -j 32 $@>&1)
}

main $@
