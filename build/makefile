BOOTLOADERDIR = ../bootloader
KERNELDIR = ../kernel
FILESYSTEMDIR= ./filesystem
BUILDDIR = .

all: build run-dev

build:
	make -C $(BOOTLOADERDIR)
	make -C $(KERNELDIR)

run-dev:
	qemu-system-x86_64 -cpu qemu64 -m 512M -bios $(BUILDDIR)/bios.bin -net none -hda fat:rw:$(FILESYSTEMDIR)
