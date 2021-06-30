ARCH=x86_64
TARGET = bootloader.efi
USE_GCC = 1
CFLAGS=
LDFLAGS=
LIBS= #static only .a

BUILDDIR=../build

bootloader: all
	mv bootloader.efi $(BUILDDIR)
	mv bootloader.o $(BUILDDIR)

include uefi/Makefile
