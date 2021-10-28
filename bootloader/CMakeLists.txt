###########
# Project #
###########

# minimum cmake version
cmake_minimum_required(VERSION 3.21)

# project general options
project(os-dev-bootloader C)

# posix-uefi needed compilation files
set(POSIXUEFI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../toolchain/posix-uefi/uefi")

# enable ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

#############
# Toolchain #
#############

# Set the compiler & linker (x86_64-elf gcc)
set(CMAKE_C_COMPILER "${TOOLCHAINBIN}/x86_64-elf-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAINBIN}/x86_64-elf-g++")
set(CMAKE_LINKER "${TOOLCHAINBIN}/x86_64-elf-ld")
set(CMAKE_OBJCOPY "${TOOLCHAINBIN}/x86_64-elf-objcopy")

####################
# Compiler options #
####################

# Sane compiler options for a kernel
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding")       # Indicate freestanding enviroment
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fshort-wchar")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --ansi")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector") # Disable stack canary      
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-check")     # Disable stack canary check
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_USE_MS_ABI")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__x86_64__")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mno-red-zone")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-builtin-declaration-mismatch")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--defsym=_DYNAMIC=0")

# Extra compiler options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")

# Set minimum C standard 
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

##################
# Linker options #
##################

# linker location
set(LINKER_SCRIPT "${POSIXUEFI_DIR}/elf_x86_64_efi.lds")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -shared")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Bsymbolic")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Luefi")

# linker options
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

###############
# Get headers #
###############

# uefi headers
set(UEFI_HEADER_DIR
	${POSIXUEFI_DIR}
)

# EFI headers
set(EFI_HEADER_DIR
	/usr/include
	/usr/include/efi
	/usr/include/efi/protocol
	/usr/include/efi/x86_64
)

# bootloader headers
set(BOOTLOADER_HEADER_DIR
	lib	
)

# merge all header directories
set(INCLUDE_DIRECTORIES
	${UEFI_HEADER_DIR}
	${EFI_HEADER_DIR}
	${BOOTLOADER_HEADER_DIR}
)

# set include directories
include_directories(${INCLUDE_DIRECTORIES})

###############
# Get sources #
###############

# Helper script
# find folder/ -print | grep .cpp | cut -c 3-

# bootloader sources
set(BOOTLOADER_SOURCES
	bootloader.c
	lib/elf/loader.c
	lib/gop/font.c
	lib/gop/framebuffer.c
	lib/gop/gop.c
	lib/memory/memory.c
	lib/io/file.c
)

# posix-uefi libs
set(POSIXUEFI_LIBS
	${POSIXUEFI_DIR}/crt_x86_64.o
	${POSIXUEFI_DIR}/libuefi.a
)

# set source files
set(SOURCES
	${BOOTLOADER_SOURCES}
	${POSIXUEFI_LIBS}
)

###############
# Compilation #
###############

# Final executable
add_executable(bootloader ${POSIXUEFI_LIBS} ${BOOTLOADER_SOURCES})

SET_SOURCE_FILES_PROPERTIES(
  ${POSIXUEFI_LIBS}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

# Executable with .elf suffix
set_target_properties(bootloader PROPERTIES
  SUFFIX ""
)

# Custom linker script
set_target_properties(bootloader PROPERTIES 
	LINK_DEPENDS ${LINKER_SCRIPT}
)

# Using system's objcopy instead the toolchain's one as it doesn't have the needed target
add_custom_command(TARGET bootloader POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 "$<TARGET_FILE:bootloader>" "$<TARGET_FILE:bootloader>.efi" 
	COMMAND rm "$<TARGET_FILE:bootloader>" 
	VERBATIM)
