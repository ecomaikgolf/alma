BOOTLOADERDIR = ../bootloader
KERNELDIR = ../kernel
FILESYSTEMDIR= ./system
BUILDDIR = .
QEMUPARAMS = -cpu qemu64 -m 256M -bios $(BUILDDIR)/bios.bin -net none -hda fat:rw:$(FILESYSTEMDIR)

all: build run

build:
	make -C $(BOOTLOADERDIR)
	make -C $(KERNELDIR)

run:
	qemu-system-x86_64 $(QEMUPARAMS)

debug: build
	qemu-system-x86_64 -s -S $(QEMUPARAMS)

clean:
	make -C $(BOOTLOADERDIR) clean
	make -C $(KERNELDIR) clean
