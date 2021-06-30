BUILDDIR=build
FILESYSTEMDIR=$(BUILDDIR)/filesystem

all: dev-run 

dev-run: 
	qemu-system-x86_64 -m 512 -bios $(BUILDDIR)/bios.bin -net none -hda fat:rw:$(FILESYSTEMDIR)
