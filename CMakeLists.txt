cmake_minimum_required(VERSION 3.21)

project(os-dev)

# Toolchain directories (x86_64-elf target gcc toolchain)
set(TOOLCHAINDIR "${CMAKE_CURRENT_SOURCE_DIR}/toolchain")
set(TOOLCHAINBIN "${TOOLCHAINDIR}/build/toolchain/bin")

# Required sowftware
find_program(NASM nasm REQUIRED)

add_subdirectory(kernel)
