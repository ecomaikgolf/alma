FILESYSTEMDIR= .
QEMUPARAMS = -cpu qemu64 -m 256M -bios $(FILESYSTEMDIR)/bios.bin -net none 
QEMUHDD = -hda fat:rw:$(FILESYSTEMDIR)
IMGFILE = flash.img
DEFDEV = /dev/sda

FILES = bootloader.efi kernel.elf zap-light16.psf
QEMU_FILES: startup.nsh bios.bin
QEMU_DEV: bios.bin
ALL_FILES = $(FILES) $(QEMU_FILES) $(QEMU_DEV) 

all: run

run: $(ALL_FILES)
	qemu-system-x86_64 $(QEMUPARAMS) $(QEMUHDD)

debug:
	qemu-system-x86_64 -s -S $(QEMUPARAMS) $(QEMUHDD) #-serial stdio > log.txt

img: $(FILES)
	rm -f $(IMGFILE)
	dd if=/dev/zero of=$(IMGFILE) bs=1k count=1440
	mformat -i $(IMGFILE) -f 1440 ::
	mmd -i $(IMGFILE) ::/EFI
	mmd -i $(IMGFILE) ::/EFI/BOOT
	mcopy -i $(IMGFILE) bootloader.efi ::/EFI/BOOT/BOOTX64.EFI
	mcopy -i $(IMGFILE) kernel.elf ::
	mcopy -i $(IMGFILE) zap-light16.psf ::
	@echo
	@echo "Done. Now you can flash it to a USB like:"
	@echo "> dd if=$(IMGFILE) of=/dev/sdX bs=1M"
	@echo "Remember to boot in UEFI mode"

flash: $(IMGFILE)
	@echo "README ==== ¿ is $(DEFDEV) CORRECT ? (3x Enter) ==== "
	@echo "THIS IS VERY IMPORTANT DON'T CONTINUE IF YOU DON'T KNOW WHAT ARE YOU DOING"
	@echo "$(DEFDEV) device is going to be erased, check that it isn't any internal HDD"
	read aux && \
	read aux && \
	read aux && \
	sudo dd if=$(IMGFILE) of=$(DEFDEV) bs=1M
	@echo "Done. Remember to boot in UEFI mode"

dev: bios.bin
	@echo "PLEASE CHECK THAT $(DEFDEV) IS YOUR USB DRIVE"
	@echo "THIS IS VERY IMPORTANT DON'T CONTINUE IF YOU DON'T KNOW WHAT ARE YOU DOING"
	read aux && \
	sudo qemu-system-x86_64 $(QEMUPARAMS) $(DEFDEV)


$(ALL_FILES) : 
	$(error Missing files. Remember to build the project)
