# Toolchain

> Dependencies to build the project

## edk2

> EDK II is a modern, feature-rich, cross-platform firmware development environment for the UEFI and PI specifications.

[https://github.com/tianocore/edk2](https://github.com/tianocore/edk2)

Using the [OVMF](https://github.com/tianocore/edk2/blob/master/OvmfPkg/README) from the edk2 project.

### edk2 OVMF

> The Open Virtual Machine Firmware (OVMF) project aims to support firmware for Virtual Machines using the edk2 code base.

**Current capabilities:**

* IA32 and X64 architectures
* QEMU (version 1.7.1 or later, with 1.7 or later machine types)
  - Video, keyboard, IDE, CD-ROM, serial
  - Runs UEFI shell
  - Optional NIC support.
* UEFI Linux boots
* UEFI Windows 8 boots
* UEFI Windows 7 & Windows 2008 Server boot

## binutils-gdb

> The GNU Binutils are a collection of binary tools.

**Some tools:**

* ld - the GNU linker.
* as - the GNU assembler
* objdump - Displays information from object files.
* readelf - Displays information from any ELF format object file.
* strip - Discards symbols.
* size - Lists the section sizes of an object or archive file.
* strings - Lists printable strings from files.

Needed to build gcc from source

## gcc

> The GNU Compiler Collection

Built for target `x86_64-elf`

Compiled with the following options:

```
--disable-nls: Disable other languages support (only English output)
--enable-languages: Build support for c,c++ only
--without-headers: Tells GCC not to rely on any C library (standard or runtime) present for the target
```

And removed the [red zone](https://wiki.osdev.org/Libgcc_without_red_zone) from libgcc.

Red zone:

```
It is a 128 byte long region located directly below the stack pointer. This region is free-for-use for the compiler without the requirement to notify the application / the os or any running interrupt handler.

For user applications there is no issue as interrupts and other kernel related code won't interfere with the user stack. In your kernel however things can get ugly, especially so if you have nested interrupts and no red-zone support. Imagine running inside your interrupt handler, gcc puts some data inside the red zone, a nested interrupt occurs and clobbers the red-zone or vice versa.
```

## posix-uefi

> Dependency-free POSIX compatibility layer for UEFI

[https://gitlab.com/bztsrc/posix-uefi](https://gitlab.com/bztsrc/posix-uefi)

```
An UEFI environment consist of two parts: a firmware with GUID protocol interfaces and a user library. We cannot change the former, but we can make the second friendlier. That's what POSIX-UEFI does for your application. It is a small API wrapper library around the GUID protocols, not a fully blown POSIX compatible libc implementation.
```

It's a layer to access UEFI functionalities in a posix way.
