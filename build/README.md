# Build Directory

```
.
├── bios.bin -> ../toolchain/build/uefi/bios.bin
├── bootloader
│  └── *.o
├── bootloader.efi
├── kernel
│  └── *.o
├── kernel.elf
├── makefile
└── system
   ├── bootloader.efi -> ../bootloader.efi
   ├── kernel.elf -> ../kernel.elf
   ├── startup.nsh -> ../../bootloader/startup.nsh
   └── zap-light16.psf -> ../../toolchain/font/zap-light16.psf
```

## Build Data
* **bios.bin**: OVMF
* **bootloader/.o**: bootloader compilation units
* **bootloader.efi**: bootloader binary
* **kernel/.o**: kernel compilation units
* **bootloader.efi**: kernel binary

## Operating System Data
* **system/bootloader.efi**: symlink to bootloader binary
* **system/kernel.efi**: symlink to kernelbinary
* **system/startup.nsh**: symlink to UEFI autostart script
* **system/zap-light16.psf**: symlink to PSF font

