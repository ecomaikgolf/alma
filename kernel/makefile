#
CC=$(TOOLCHAINBIN)/x86_64-elf-g++
CFLAGS= -ffreestanding -fno-stack-protector -fno-stack-check -Wall -Wextra -I$(INCDIR) -g
INCDIR=.
LD=$(TOOLCHAINBIN)/x86_64-elf-ld
LDFLAGS= -T kernel.ld -nostdlib -z max-page-size=0x1000
ASMC=nasm
ASMFLAGS=

BUILDDIR=../build/kernel
TOOLCHAINDIR=../toolchain
TOOLCHAINBIN=$(TOOLCHAINDIR)/build/toolchain/bin

NAME=kernel.elf

SRCS=$(wildcard *.cpp */*.cpp)
HDRS=$(wildcard *.h */*.h)
ASMSRC=$(wildcard *.asm */*.asm)
TMP=$(SRCS:.cpp=.o)
TMP+=$(ASMSRC:.asm=_asm.o)
OBJS=$(patsubst %, $(BUILDDIR)/%, $(TMP))

all: link kernel.ld makefile

$(BUILDDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$(notdir $@) -c $<

$(BUILDDIR)/%_asm.o: %.asm
	$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $(BUILDDIR)/$(notdir $@)

link: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/../kernel.elf $(patsubst %, $(BUILDDIR)/%, $(notdir $(OBJS)))

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/../$(NAME)

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
#
#  -c 
#
#  The -c option says not to run the linker
