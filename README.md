<p align="center">
  <img width="300" height="100" src="https://ls.ecomaikgolf.com/alma/logo.png">
</p>

<p align="center">
  <i>alma is an toy kernel written in C/C++ for x86_64 machines with the mere purpose of learning OS development</i>
</p>

# alma

> 6. f. Sustancia o parte principal de cualquier cosa.

## Features

- structured project
	- simple cmake builds
	- `cmake -B build; cmake --build build` to compile
	- `make -C qemu-fs` to run
		- documented (bachelor's final project)
	- out of source build
	- build releases (no need to compile from source)
		- more or less sane git commits
	- toolchain compilation
		- `make -C toolchain`
		- git submodules
- toolchain
	- single script install (tested on archlinux)
	- compile from source
		- binutils
		- gcc
			- disable red zone patch
			- only c/c++ compiler
			- without standard/runtime headers
			- x86_64-elf target
		- edk2 ovmf
			- proper compilation with gcc5
			- release build
		- posix-uefi libs
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

0. Install `qemu-system-x86_64` binary (usually `qemu` + `qemu-arch-extra`)
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

## Tinker

`TODO`

## Contributing

Just_Fork_It™

However i'm not looking for contributors, this project is just for learning 
purposes.

To see how to work with the source code read the section "Tinker"

## Bugs

Explain them to me please: `me@ecomaikgolf.com`

## Bibliography

* https://wiki.osdev.org/Main_Page
* https://youtu.be/NpcGMuI7hxk
* https://0xax.gitbooks.io/linux-insides/content/Interrupts/
* https://youtu.be/mpPbKEeWIHU
* https://www.iaik.tugraz.at/teaching/materials/os/tutorials/paging-on-intel-x86-64/
* https://git.musl-libc.org/cgit/musl/tree

_The list would be longer but I can't list each site where I read something. This list is a good place to start_

## Author

Ernesto Martínez García
* me@ecomaikgolf.com _(C79F 01CE 017F 57A4 FBBB 4E22 33DD FB0A EB94 20CB)_
* emg162@alu.ua.es 
  [[verify](https://github.com/leereilly/swot/blob/master/lib/domains/es/ua.txt)]
* https://ecomaikgolf.com/
* https://twitter.com/ecomaikgolf
