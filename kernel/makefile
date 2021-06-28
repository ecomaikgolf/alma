CC=x86_64-elf-gcc
CFLAGS= -ffreestanding
LD=ld
LDFLAGS= -nostdlib -static #-Bsymbolic

all: link

kernel.o: kernel.c
	$(CC) $(CFLAGS) -o kernel.o -c kernel.c

link: kernel.o
	$(LD) $(LDFLAGS) -o kernel.elf kernel.o -T kernel.ld

# -ffreestanding
#
# Assert that compilation targets a freestanding environment. This implies -fno-builtin.  A freestanding
# environment is one in which the standard library may not exist, and program startup may not necessarily be
# at "main".  The most obvious example is an OS kernel.  This is equivalent to -fno-hosted.
#
# -nostdlib
#
#  Only search library directories explicitly specified on the command line.  Library directories specified
#  in linker scripts (including linker scripts specified on the command line) are ignored.
#
# -static
#
#  The -static flag forces the linker to accept only static libraries and not any shared libraries.
#
#  -Bsymbolic
#
#  https://flameeyes.blog/2012/10/07/symbolism-and-elf-files-or-what-does-bsymbolic-do/
