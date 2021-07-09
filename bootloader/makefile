ARCH=x86_64
TARGET = bootloader.efi
USE_GCC = 1
CFLAGS=-I$(LIBDIR)
LDFLAGS=
LIBS= #static only .a
LIBDIR=lib

BUILDDIR=../build

bootloader: all
	mv bootloader.efi $(BUILDDIR)
	mv bootloader.o $(BUILDDIR)

include uefi/Makefile
