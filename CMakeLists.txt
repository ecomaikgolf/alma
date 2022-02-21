#####################
# alma build system #
#####################

# cmake >= 3.16 (default in Ubuntu 20.04)
cmake_minimum_required(VERSION 3.16)

project(alma)

# software requirements
find_program(MFORMAT mformat REQUIRED)
find_program(MMD mmd REQUIRED)
find_program(MCOPY mcopy REQUIRED)
find_program(QEMU qemu-system-x86_64 REQUIRED)

# Toolchain directories (x86_64-elf target gcc toolchain)
set(TOOLCHAINDIR "${CMAKE_CURRENT_SOURCE_DIR}/toolchain")
set(TOOLCHAINBIN "${TOOLCHAINDIR}/build/toolchain/bin")

# qemu configuration
set(QEMU_BIN qemu-system-x86_64)
set(QEMU_CPU -cpu qemu64)
set(QEMU_MACH -machine q35)
set(QEMU_RAM -m 256M)
set(QEMU_BIOS -bios ${TOOLCHAINDIR}/build/uefi/bios.bin)
set(QEMU_NET -nic user,model=rtl8139,mac=ca:fe:c0:ff:ee:00)
set(QEMU_BOOT -drive file=${CMAKE_BINARY_DIR}/disk.img,format=raw,readonly=on,id=cd,if=none -device ahci,id=achi0 -device ide-cd,bus=achi0.0,drive=cd,id=cd1,bootindex=1)
set(QEMU_DBG -s -S)

# Use this file to improve LSP accuracy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Submodules
add_subdirectory(kernel)
add_subdirectory(bootloader)

# system image generation
add_custom_command(OUTPUT disk.img
  DEPENDS kernel bootloader
  COMMAND dd if=/dev/zero of=disk.img bs=1k count=1440
  COMMAND mformat -i disk.img -f 1440 ::
  COMMAND mmd -i disk.img ::/EFI
  COMMAND mmd -i disk.img ::/EFI/BOOT
  COMMAND mcopy -n -o -i disk.img bootloader/bootloader.efi ::/EFI/BOOT/BOOTX64.EFI
  COMMAND mcopy -n -o -i disk.img $<TARGET_FILE:kernel> ::
  COMMAND mcopy -n -o -i disk.img ${TOOLCHAINDIR}/font/zap-light16.psf ::
  VERBATIM
)

# (default) system image generation (patch to avoid rebuilding)
add_custom_target(disk ALL
  DEPENDS disk.img
)

# 'run' qemu emulation
add_custom_target(run
  DEPENDS disk
  COMMAND ${QEMU_BIN} ${QEMU_MACH} ${QEMU_CPU} ${QEMU_RAM} ${QEMU_BIOS} ${QEMU_NET} ${QEMU_BOOT}
  VERBATIM
)

# 'debug' qemu debug
add_custom_target(debug
  DEPENDS disk
  COMMAND ${QEMU_BIN} ${QEMU_DBG} ${QEMU_MACH} ${QEMU_CPU} ${QEMU_RAM} ${QEMU_BIOS} ${QEMU_NET} ${QEMU_BOOT} &; gdb -ex "target remote localhost:1234" $<TARGET_FILE:kernel>
  USES_TERMINAL
)

# doxygen documentation generation
find_program(DOXYGEN doxygen)
if (DOXYGEN)
  # 'doc' Generate documentation in build dir
  set(DOXYFILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    add_custom_target(doc ALL
        COMMAND doxygen ${DOXYFILE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        VERBATIM )
  # 'pdoc' Generate documentation in project dir
  set(DOXYFILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    add_custom_target(pdoc ALL
        COMMAND doxygen ${DOXYFILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM )
endif (DOXYGEN)
