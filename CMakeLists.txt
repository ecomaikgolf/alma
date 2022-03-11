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
set(QEMU_NET -nic user,model=rtl8139,mac=ca:fe:c0:ff:ee:00 -netdev user,id=u1 -device e1000,netdev=u1 -object filter-dump,id=f1,netdev=u1,file=dump.dat)
set(QEMU_BOOT -boot d -cdrom ${CMAKE_BINARY_DIR}/alma.iso)
set(QEMU_DBG -s -S)

# Use this file to improve LSP accuracy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Submodules
add_subdirectory(kernel)
# add_subdirectory(bootloader) (DEPRECATED)

# system image generation (DEPRECATED)
#add_custom_command(OUTPUT disk.img
#  DEPENDS kernel bootloader
#  COMMAND dd if=/dev/zero of=disk.img bs=1k count=1440
#  COMMAND mformat -i disk.img -f 1440 ::
#  COMMAND mmd -i disk.img ::/EFI
#  COMMAND mmd -i disk.img ::/EFI/BOOT
#  COMMAND mcopy -n -o -i disk.img bootloader/bootloader.efi ::/EFI/BOOT/BOOTX64.EFI
#  COMMAND mcopy -n -o -i disk.img $<TARGET_FILE:kernel> ::
#  COMMAND mcopy -n -o -i disk.img ${TOOLCHAINDIR}/font/zap-light16.psf ::
#  VERBATIM
#)

# system image generation (stivale version)
add_custom_command(OUTPUT alma.iso
  DEPENDS kernel ${CMAKE_CURRENT_SOURCE_DIR}/limine.cfg
  COMMAND rm -rf iso_root
  COMMAND mkdir -p iso_root
  COMMAND cp -v $<TARGET_FILE:kernel> ${CMAKE_CURRENT_SOURCE_DIR}/limine.cfg ${TOOLCHAINDIR}/limine/limine.sys ${TOOLCHAINDIR}/limine/limine-cd.bin ${TOOLCHAINDIR}/limine/limine-eltorito-efi.bin ${TOOLCHAINDIR}/font/zap-light16.psf iso_root/
  COMMAND xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-eltorito-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o alma.iso
  COMMAND ${TOOLCHAINDIR}/limine/limine-install alma.iso
  VERBATIM
)

# (default) system image generation (patch to avoid rebuilding)
add_custom_target(iso ALL
  DEPENDS alma.iso
)

# 'run' qemu emulation
add_custom_target(run
  DEPENDS iso
  COMMAND ${QEMU_BIN} ${QEMU_MACH} ${QEMU_CPU} ${QEMU_RAM} ${QEMU_BIOS} ${QEMU_NET} ${QEMU_BOOT}
  VERBATIM
)

# 'debug' qemu debug
add_custom_target(debug
  DEPENDS iso
  COMMAND ${QEMU_BIN} ${QEMU_DBG} ${QEMU_MACH} ${QEMU_CPU} ${QEMU_RAM} ${QEMU_BIOS} ${QEMU_NET} ${QEMU_BOOT} &; gdb -ex "target remote localhost:1234" $<TARGET_FILE:kernel>
  USES_TERMINAL
)

# doxygen documentation generation
find_program(DOXYGEN doxygen)
if (DOXYGEN)
  set(DOXYFILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    add_custom_target(doc
        COMMAND doxygen ${DOXYFILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM )
endif (DOXYGEN)
