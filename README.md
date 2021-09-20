# os-dev

## Build from source

### Install toolchain dependencies

Install the following packages from your package manager of choice:

* nasm
* qemu
* qemu-arch-extra
* cmake
* make

Then compile the toolchain for the project:

```bash
make -C toolchain
```

Compiles:

* binutils
* edk2
* gcc (x86_64-elf)
* posix-uefi

Note: It's a hard task to get a bulletproof script/makefile to compile these 
projects so I apply the "it works on my machine" law.

### Run the project

```bash
cmake -B build  # Generate makefiles
make -C build   # Compile project
make -C qemu-fs # Run
```
