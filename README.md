# os-dev

## Build from source

### Install toolchain dependencies

> Note: It's a hard task to get a bulletproof script/makefile to compile the entire toolchain so I apply the "it works on my machine" law.

Install the following packages from your package manager of choice:

* nasm
* qemu
* qemu-arch-extra
* acpica (`iasl` tool)
* gcc-5 (you can skip it, read below)
* cmake
* make
* gcc (**warning**: i've encountered problems using other compilers like `tcc`)

Then,

to compile the entire toolchain:
```bash
make -C toolchain
```

to skip "uefi" build and consecuently don't having to install gcc-5 dependency:
```bash
make -C toolchain skip_edk2
wget -P toolchain/build/uefi https://ecomaikgolf.com/assets/bios.bin
```

Compiles:

* binutils
* edk2 (depends)
* gcc (x86_64-elf target)
* posix-uefi

### Run the project

```bash
cmake -B build  # Generate makefiles
make -C build   # Compile project
make -C qemu-fs # Run
```

## Tinker source code

### Fix LSP

To get your LSP working with the project get the `compile_commands.json` from 
the build directory (build the project at least once).

You should know what to do with it, if it's not the case, try copying it to the 
project root directory.
