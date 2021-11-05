FILESYSTEMDIR= .
QEMUPARAMS = -cpu qemu64 -m 256M -bios $(FILESYSTEMDIR)/bios.bin -net none -hda fat:rw:$(FILESYSTEMDIR)
IMGNAME=flash.img

all: run
files = kernel.elf bootloader.efi zap-light16.psf
qemu_files = bios.bin startup.nsh
all_files = $(files) $(qemu_files)

run: $(all_files)
	qemu-system-x86_64 $(QEMUPARAMS)

debug: $(all_files)
	qemu-system-x86_64 -s -S $(QEMUPARAMS) #-serial stdio > log.txt

img: $(files)
	rm -f $(flash.img)
	dd if=/dev/zero of=$(IMGNAME) bs=1k count=1440
	mformat -i $(IMGNAME) -f 1440 ::
	mmd -i $(IMGNAME) ::/EFI
	mmd -i $(IMGNAME) ::/EFI/BOOT
	mcopy -i $(IMGNAME) bootloader.efi ::/EFI/BOOT/BOOTX64.EFI
	mcopy -i $(IMGNAME) kernel.elf ::
	mcopy -i $(IMGNAME) zap-light16.psf ::
	@echo
	@echo "Done, now you can flash it to a USB drive like:"
	@echo "> dd if=/dev/sdX of=$(IMGNAME) bs=1M"
	@echo "Remember to boot it with UEFI"

$(all_files):
	$(error Missing files, please compile the project)

