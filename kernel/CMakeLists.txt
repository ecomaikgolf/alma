###########
# Project #
###########

# minimum cmake version
cmake_minimum_required(VERSION 3.16)

project(alma-kernel CXX ASM_NASM)

# nasm is required to build the kernel
find_program(NASM nasm REQUIRED)

# enable ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-red-zone") 	   # Disable red zone
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") 	   			   # Debug builds
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-4") 	   	   # Use dwarf 4 for bloaty
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcmodel=kernel") 	   # limine + stivale mandates it

# Extra compiler options
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")

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
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static")  # static
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

###############
# Get headers #
###############

# libc headers. to do just `#include "math.h"`
set(LIB_HEADER_DIR
	lib
)

# stivale2 headers
set(STIVALE_HEADER_DIR
	${TOOLCHAINDIR}/stivale
)

# kernel headers. to do `#include "paging/PFA.h"`
set(KERNEL_HEADER_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
)

# merge all header directories
set(INCLUDE_DIRECTORIES
	${LIB_HEADER_DIR}
	${STIVALE_HEADER_DIR}
	${KERNEL_HEADER_DIR}
)

# set include directories
include_directories(${INCLUDE_DIRECTORIES})

###############
# Get sources #
###############

# Helper script
# find folder/ -print | grep .cpp | cut -c 3-

# lib sources
set(LIB_SOURCES
	lib/stdlib/itoa.cpp
	lib/stdlib/strol.cpp
	lib/string/memset.cpp
	lib/string/strcmp.cpp
	lib/string/strlen.cpp
	lib/math/pow.cpp
	lib/math/sqrt.cpp
	lib/math/log.cpp
	lib/ctype/toupper.cpp
	lib/bitset.cpp
)

# interrupt sources
set(INTERRUPT_SOURCES
	interrupts/IDT.cpp
	interrupts/interrupts.cpp
)

# kernel sources
set(KERNEL_SOURCES
	bootstrap/startup.cpp
	paging/PFA.cpp
	paging/BPFA.cpp
	paging/PTM.cpp
	screen/simple_renderer_i.cpp
	screen/fast_renderer_i.cpp
	uefi/memory.cpp
	segmentation/gdt.asm
	io/bus.cpp
	io/keyboard.cpp
	acpi/acpi.cpp
	pci/pci.cpp
	heap/simple_allocator.cpp
	heap/trivial_allocator.cpp
	shell/command.cpp
	shell/interpreter.cpp
	net/rtl8139.cpp
	${INTERRUPT_SOURCES}
	kernel.cpp
)

# set source files
set(SOURCES
	${KERNEL_SOURCES}
	${LIB_SOURCES}
)

###############
# Compilation #
###############

# Special compile options for interrupts
set_source_files_properties(${INTERRUPT_SOURCES} PROPERTIES COMPILE_FLAGS "-mno-red-zone -mgeneral-regs-only -ffreestanding")

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

###############
# _init _fini #
###############

# build crts
add_library(crts OBJECT
    crti.asm
    crtn.asm
)

# build crts first
add_dependencies(kernel crts)

# retrieve crtbegin.o and crtend.o
execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtbegin.o OUTPUT_VARIABLE CRTBEGIN_O OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtend.o OUTPUT_VARIABLE CRTEND_O OUTPUT_STRIP_TRAILING_WHITESPACE)

# kernel linking (as it has to be done in specific order)
# yes i know that i shouldn't be using a hardcoded build path :(
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} <LINK_FLAGS> ${CMAKE_BINARY_DIR}/kernel/CMakeFiles/crts.dir/crti.asm.o ${CRTBEGIN_O} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> ${CRTEND_O} ${CMAKE_BINARY_DIR}/kernel/CMakeFiles/crts.dir/crtn.asm.o" )
