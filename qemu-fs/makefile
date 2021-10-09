FILESYSTEMDIR= .
QEMUPARAMS = -cpu qemu64 -m 256M -bios $(FILESYSTEMDIR)/bios.bin -net none -hda fat:rw:$(FILESYSTEMDIR)

all: run

run:
	qemu-system-x86_64 $(QEMUPARAMS)

debug:
	qemu-system-x86_64 -s -S $(QEMUPARAMS) #-serial stdio > log.txt
