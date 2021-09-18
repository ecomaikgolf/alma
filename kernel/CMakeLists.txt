###########
# Project #
###########

# project general options
project(os-dev-kernel CXX ASM_NASM)

# nasm is required to build the kernel
find_program(NASM nasm REQUIRED)

#############
# Toolchain #
#############

# Set the compiler & linker (x86_64-elf gcc)
set(CMAKE_C_COMPILER "${TOOLCHAINBIN}/x86_64-elf-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAINBIN}/x86_64-elf-g++")
set(CMAKE_LINKER "${TOOLCHAINBIN}/x86_64-elf-ld")

####################
# Compiler options #
####################

# Sane compiler options for a kernel
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffreestanding")       # Indicate freestanding enviroment
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-protector") # Disable stack canary      
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-check")     # Disable stack canary check
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")            # Disable C++ run-time type information
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions") 	   # Disable exceptions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib") 		   # Disable std lib

# Extra compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")

# Set minimum CXX standard 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Disable CXX extensions
set(CMAKE_CXX_EXTENSIONS OFF)

#####################
# Assembler options #
#####################

# indicate nasm output format
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

##################
# Linker options #
##################

# linkscript location
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld")

# linker options
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -nostdlib") 				  # Disable stdlib
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -z max-page-size=0x1000")  # Set maximum page size
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

###############
# Get headers #
###############

# libc headers. to do just `#include "math.h"`
set(LIBC_HEADER_DIR
	libc
)

# libstdc++ headers. to do just `#include "bitset.h"`
set(LIBSTDCXX_HEADER_DIR
	libstdc++
)

# kernel headers. to do `#include "paging/PFA.h"`
set(KERNEL_HEADER_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
)

# merge all header directories
set(INCLUDE_DIRECTORIES
	${LIBC_HEADER_DIR}
	${LIBSTDCXX_HEADER_DIR}
	${KERNEL_HEADER_DIR}
)

# set include directories
include_directories(${INCLUDE_DIRECTORIES})

###############
# Get sources #
###############

# Helper script
# find folder/ -print | grep .cpp | cut -c 3-

# libc sources
set(LIBC_SOURCES
	libc/stdlib/itoa.cpp
	libc/string/memset.cpp
	libc/math/pow.cpp
	libc/math/sqrt.cpp
	libc/math/log.cpp
)

# libstdc++ sources
set(LIBSTDCXX_SOURCES
	libstdc++/bitset.cpp
)

# kernel sources
set(KERNEL_SOURCES
	paging/PFA.cpp
	paging/PTM.cpp
	screen/renderer.cpp
	uefi/memory_map.cpp
	segmentation/gdt.asm
	kernel.cpp
)

# set source files
set(SOURCES
	${KERNEL_SOURCES}
	${LIBC_SOURCES}
	${LIBSTDCXX_SOURCES}
)

###############
# Compilation #
###############

# Final executable
add_executable(kernel ${SOURCES})

# Executable with .elf suffix
set_target_properties(kernel PROPERTIES
  SUFFIX .elf
)

# Custom linker script
set_target_properties(kernel PROPERTIES 
	LINK_DEPENDS ${LINKER_SCRIPT}
)
