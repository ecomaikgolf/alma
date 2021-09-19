cmake_minimum_required(VERSION 3.21)

project(os-dev)

# Toolchain directories (x86_64-elf target gcc toolchain)
set(TOOLCHAINDIR "${CMAKE_CURRENT_SOURCE_DIR}/toolchain")
set(TOOLCHAINBIN "${TOOLCHAINDIR}/build/toolchain/bin")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build")
add_subdirectory(kernel)
add_subdirectory(bootloader)
