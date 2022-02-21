cmake_minimum_required(VERSION 3.16)

project(alma)

set(QEMU_FS "${CMAKE_CURRENT_SOURCE_DIR}/qemu-fs")

# Toolchain directories (x86_64-elf target gcc toolchain)
set(TOOLCHAINDIR "${CMAKE_CURRENT_SOURCE_DIR}/toolchain")
set(TOOLCHAINBIN "${TOOLCHAINDIR}/build/toolchain/bin")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(kernel)
add_subdirectory(bootloader)

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

add_custom_target(disk ALL
  DEPENDS disk.img
)
