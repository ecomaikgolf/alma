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
	mv bootloader.o $(BUILDDIR)/bootloader

include uefi/Makefile

clean:
	$(MAKE) -f uefi/Makefile $@  # https://stackoverflow.com/a/1645087
	rm -f $(BUILDDIR)/bootloader.efi $(BUILDDIR)/bootloader/*.o $(LIBDIR)/*.o $(LIBDIR)/*/*.o
