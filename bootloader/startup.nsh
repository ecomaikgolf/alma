@echo -off
mode 80 25

cls
echo "(I) [startup.nsh] bootloader.efi finding started"

if exist fs0:\bootloader.efi then
	fs0:
	echo "(I) [startup.nsh] bootloader.efi found in fs0:\bootloader.efi"
	.\bootloader.efi
 goto END
endif

echo "(E) [startup.nsh] bootloader.efi not found"
 
:END
echo "(I) [startup.nsh] bootloader.efi ended"
