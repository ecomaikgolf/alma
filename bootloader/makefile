###########
# General #
###########
THREADS=8
BUILD_DIR=build

########
# edk2 #
########
PLATFORM=OvmfPkg/OvmfPkgX64.dsc
PLATFORM_NAME=OvmfX64
TARGET=RELEASE
ARCH=X64
TOOLCHAIN=GCC5
COMPIL=gcc-5
BIOS_FILE=bios.bin

.PHONY: build edk2 edk2-test clean posix-uefi

build: edk2 posix-uefi

edk2: $(BUILD_DIR)/bios.bin

$(BUILD_DIR)/bios.bin: 
	git submodule update --init
	cd edk2; \
	git submodule update --init; \
	make CC=$(COMPIL) -j$(THREADS) -C BaseTools; \
	source ./edksetup.sh; \
	sed -i -e "s@= EmulatorPkg/EmulatorPkg.dsc@= $(PLATFORM)@" Conf/target.txt; \
	sed -i -e "s@= DEBUG@= $(TARGET)@" Conf/target.txt; \
	sed -i -e "s@= IA32@= $(ARCH)@" Conf/target.txt; \
	sed -i -e "s@= VS2015x86@= $(TOOLCHAIN)@" Conf/target.txt; \
	CC=$(COMPIL) build; \
	cp ./Build/$(PLATFORM_NAME)/$(TARGET)_$(TOOLCHAIN)/FV/OVMF.fd ../$(BUILD_DIR)/$(BIOS_FILE);
	@echo -e "Generated $(BIOS_FILE)"

edk2-test: $(BUILD_DIR)/bios.bin
	emu-system-x86_64 -bios $(BUILD_DIR)/$(BIOS_FILE) -net none

posix-uefi:
	git submodule update --init
	USE_GCC=1 make -C posix-uefi/ #No need, just checks
	ln -s posix-uefi/uefi build/uefi

clean:
	rm -rf edk2/Build/
	#git checkout $(git describe --abbrev=0 --tags) # Checkout to latest tag
