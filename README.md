<p align="center">
  <img width="300" height="100" src="https://ls.ecomaikgolf.com/alma/logo.png">
</p>

<p align="center">
  <i>alma is a toy kernel written in C/C++ for x86_64 machines with the mere purpose of learning OS development</i>
</p>

# alma

> 6. f. Sustancia o parte principal de cualquier cosa.

## Features

- project
	- compilation
		- cmake
		- `cmake -B build; cmake --build build`
		- out of source builds
	- run
		- `make -C qemu-fs`
		- `make -C qemu-fs debug` remote tcp gdb (localhost:1234)
	- documentation
		- inner workings (bachelor's final project)
		- code (doxygen + comments)
	- build releases
		- https://ls.ecomaikgolf.com/alma/builds/
	- git
		- more or less clean management
	- toolchain
		- compilation from sources
			- binutils
				- x86_64-elf target
			- gcc
				- disable red zone patch
				- only c/c++ compiler
				- disable std/runtime things
				- x86_64-elf target
			- edk2
				- release build
			- posix-uefi
				- independent from posix-uefi Makefile
		- easy toolchain instalation
			- 1: dependencies & `make -C toolchain`
			- 2: preconfigured alma build VM
		- git submodules (easy toolchain updating)
- uefi shell
	- bootloader run script
- bootloader
	- C + posix-uefi
		- commented code (doxygen)
		- compilation independent from posix-uefi's makefile (cmake)
	- loads kernel
	- parses and verifies (some) ELF headers
	- graphics output protocol from UEFI
	- framebuffer
	- uefi memory map
	- PSF1 fonts
	- exit UEFI boot services
	- jumps to kernel
- kernel
	- C++/NASM-ASM
		- commented code (doxygen)
		- structured (folders and namespaces)
	- screen/tty
		- print pixels
		- print letters with a PSF1 font
		- colors
		- printf
	- pagination
		- page frame allocator
			- request, reserve, lock... pages (bitset)
		- virtual memory
			- CPU's MMU virt-phys translation
			- 512-ary tree (4 levels)
	- segmentation
		- dummy global descriptor table (I use paging)
	- interrupts
		- interrupt descriptor table
		- register (vector, interrupt routine)
		- call interrupt routines on interrupts

## Run

0. Install dependencies (listed below)
1. Find build to test https://ls.ecomaikgolf.com/alma/builds/
2. Download a build with wget:

```bash
wget https://ls.ecomaikgolf.com/alma/builds/20211012-e906c4c.tar.gz # <-- Change build version!
tar xf 20211012-e906c4c.tar.gz
cd 20211012-e906c4c
```

3. Download precompiled edk2 Ovmf UEFI

```bash
wget https://ls.ecomaikgolf.com/alma/builds/bios.bin
```

4. Run

```bash
make
```

### Run Dependencies

* `qemu-system-x86`

Ubuntu: `apt install qemu-system-x86 qemu-system-gui`

Arch-Linux: `pacman -S qemu qemu-arch-extra`

## Build

### Virtual Machine

Get the "Alma Build VM" (.ova 5.9G), a Xubuntu 20.04 virtualbox VM ready to 
compile.

**Download (@gcloud.ua.es):** https://drive.google.com/file/d/1neuuBo7Ja4Czmwre1SWPkwGSF8MV98s3/view?usp=sharing

<p align="center">
  <img width="800" height="400" src="https://ls.ecomaikgolf.com/alma/alma-build-vm.png">
</p>

Features:
* already compiled toolchain
* systemwide ccache
* preconfigured "neutral" IDE (vscodium)
* one click functionalities
	* compile
	* run
	* update
	* clean

### Manual

Tested on `Ubuntu 20.04` 

1. Install dependencies (listed below)
2. `make -C toolchain/`
3. `cmake -DCMAKE_BUILD_TYPE=Release -B build; cmake --build build`
4. `make -C qemu-fs`

#### Build Dependencies

* nasm
* iasl
* cmake
* qemu-system-x86
* qemu-system-gui
* uuid-dev
* python
* python3-distutils
* texinfo
* bison
* flex
* mtools

Remember to have: `make` `git` `bash` `build-essential`

Ubuntu 20.04 (and similars) one-command dependencies installer:

```bash
apt install nasm iasl cmake make qemu-system-x86 qemu-system-gui git uuid-dev python python3-distutils bash texinfo bison flex build-essential mtools
```

## Bugs

Explain them to me please: `me@ecomaikgolf.com`

## FAQ

> Q: Muh tons of dependencies
> 
> A: I have to compile binutils, gdb, edk2 and posix-uefi from source

> Q: 5.9GB VM
>
> A: I can't do more. Xubuntu + "zeroed" free memory before exporting so it can compress it.

> Q: Frequently asked questions?
>
> A: More like preanswered questions

## Bibliography

* https://wiki.osdev.org/Main_Page
* https://youtu.be/NpcGMuI7hxk
* https://0xax.gitbooks.io/linux-insides/content/Interrupts/
* https://youtu.be/mpPbKEeWIHU
* https://www.iaik.tugraz.at/teaching/materials/os/tutorials/paging-on-intel-x86-64/
* https://git.musl-libc.org/cgit/musl/tree

_The list is longer but I can't put each site where I read something. This list is a good place to start_

## Author

Ernesto Martínez García
* me@ecomaikgolf.com _(C79F 01CE 017F 57A4 FBBB 4E22 33DD FB0A EB94 20CB)_
* emg162@alu.ua.es 
  [[verify](https://github.com/leereilly/swot/blob/master/lib/domains/es/ua.txt)]
* https://ecomaikgolf.com/
* https://twitter.com/ecomaikgolf
