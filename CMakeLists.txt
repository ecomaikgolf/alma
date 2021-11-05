cmake_minimum_required(VERSION 3.16)

project(os-dev)

set(QEMU_FS "${CMAKE_CURRENT_SOURCE_DIR}/qemu-fs")

# Toolchain directories (x86_64-elf target gcc toolchain)
set(TOOLCHAINDIR "${CMAKE_CURRENT_SOURCE_DIR}/toolchain")
set(TOOLCHAINBIN "${TOOLCHAINDIR}/build/toolchain/bin")

set(EXECUTABLE_OUTPUT_PATH ${QEMU_FS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(kernel)
add_subdirectory(bootloader)
