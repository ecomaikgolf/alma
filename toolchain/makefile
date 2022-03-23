###################
#### Configure ####
###################

###########
# General #
###########
THREADS=10
BUILD_DIR=$(CURDIR)/build

############
# Versions #
############
binutils_v = binutils-2_37
gcc_v = releases/gcc-11
edk2_v = origin/stable/202011
posix-uefi_v = 1431b327d2c239ba5c94a3c1fdfdddd798610c50

####################
# Toolchain Config #
####################
TARGET_BINUTILS=x86_64-elf
BUILD_DIR_TOOLCHAIN=$(BUILD_DIR)/toolchain

###############
# edk2 Config #
###############
PLATFORM=OvmfPkg/OvmfPkgX64.dsc
PLATFORM_NAME=OvmfX64
TARGET_EDK2=RELEASE
ARCH=X64
TOOLCHAIN=GCC5
COMPILER_EDK2=gcc
BIOS_FILE=bios.bin
BUILD_DIR_UEFI=$(BUILD_DIR)/uefi

#####################
# posix-uefi Config #
#####################
USE_GCC=1
BUILD_DIR_POSIX_UEFI=$(BUILD_DIR)/posix-uefi

################################
##### MAKEFILE DO NOT TOUCH ####
################################

SHELL := /bin/bash

.PHONY: all update edk2 binutils binutils-build gcc gcc-build posix-uefi posix-uefi-build limine limine-build clean debug

all: update limine binutils gcc 
skip_edk2: update binutils gcc posix-uefi

edk2: update $(BUILD_DIR_UEFI)/bios.bin
binutils: update binutils-build
gcc: update gcc-build
posix-uefi: update posix-uefi-build
limine: update limine-build

##########
# Update #
##########
update:
	git submodule update --init --recursive

############
# binutils #
############

binutils-build: 
	cd binutils-gdb; \
	git checkout $(binutils_v); \
	mkdir build; \
	cd build; \
	../configure --target=$(TARGET_BINUTILS) --enable-targets=all --disable-gdb --disable-libdecnumber --disable-readline --disable-sim --prefix="$(BUILD_DIR_TOOLCHAIN)" --with-sysroot --disable-nls --disable-werror; \
	make -j$(THREADS); \
	make install;
	
# --disable-nls: Tells binutils not to include native language support.
# --disable-werror: Don't treat warnings as errors
# --with-sysroot: Tells binutils to enable sysroot support in the cross-compiler by pointing it to a default empty directory

########
# gcc  #
########

gcc-build:
	$(eval CONTENT = tmake_file="$$\{tmake_file} i386/t-x86_64-elf")
	$(eval NUMBER = $(shell grep -F x86_64-*-elf -n $(CURDIR)/gcc/gcc/config.gcc | cut -f1 -d:))
	cd gcc; \
	git checkout $(gcc_v); \
	./contrib/download_prerequisites; \
	echo -e "MULTILIB_OPTIONS += mno-red-zone\nMULTILIB_DIRNAMES += no-red-zone" > gcc/config/i386/t-x86_64-elf; \
	sed '$(NUMBER) a $(CONTENT)' gcc/config.gcc; \
	mkdir build; \
	cd build; \
	export PATH=$(BUILD_DIR_TOOLCHAIN)/bin/:$$PATH; \
	export PATH=$(BUILD_DIR_TOOLCHAIN)/libexec/gcc/x86_64-elf/11.2.1/:$$PATH; \
	../configure --target=x86_64-elf --prefix="$(BUILD_DIR_TOOLCHAIN)" --disable-nls --enable-languages=c,c++ --without-headers; \
	make -j$(THREADS) all-gcc; \
	make -j$(THREADS) all-target-libgcc CFLAGS_FOR_TARGET='-g -O2 -mcmodel=kernel -mno-red-zone'|| true; \
 	sed -i 's/PICFLAG/DISABLED_PICFLAG/g' x86_64-elf/libgcc/Makefile; \
	make -j$(THREADS) all-target-libgcc CFLAGS_FOR_TARGET='-g -O2 -mcmodel=kernel -mno-red-zone'; \
	make install-gcc; \
	make install-target-libgcc;

# --disable-nls: Disable other languages support (only English output)
# --enable-languages: Build support for c,c++ only
# --without-headers: Tells GCC not to rely on any C library (standard or runtime) present for the target

#############
# edk2 Ovmf #
#############

$(BUILD_DIR_UEFI)/bios.bin:
	cd edk2; \
	git checkout $(edk2_v); \
	make CC=$(COMPILER_EDK2) -j$(THREADS) -C BaseTools; \
	source ./edksetup.sh; \
	sed -i -e "s@= EmulatorPkg/EmulatorPkg.dsc@= $(PLATFORM)@" Conf/target.txt; \
	sed -i -e "s@= DEBUG@= $(TARGET_EDK2)@" Conf/target.txt; \
	sed -i -e "s@= IA32@= $(ARCH)@" Conf/target.txt; \
	sed -i -e "s@= VS2015x86@= $(TOOLCHAIN)@" Conf/target.txt; \
	CC=$(COMPILER_EDK2) build; \
	cp ./Build/$(PLATFORM_NAME)/$(TARGET_EDK2)_$(TOOLCHAIN)/FV/OVMF.fd $(BUILD_DIR_UEFI)/$(BIOS_FILE);

##############
# posix-uefi #
##############

posix-uefi-build:
	cd posix-uefi; \
	git checkout $(posix-uefi_v); \
	export PATH=$(BUILD_DIR_TOOLCHAIN)/bin/:$$PATH; \
	USE_GCC=$(USE_GCC) make -C uefi; \
	ln -s $(CURDIR)/build/uefi $(BUILD_DIR_POSIX_UEFI);

posix-uefi-examples:
	$(info The makefile shows an error but everything compiles fine apparently)
	sed -i '1d' posix-uefi/Makefile; \
	sed -i -e "1s@^@OVMF = $(BUILD_DIR_UEFI)/$(BIOS_FILE)\n@" posix-uefi/Makefile; \
	sed -i -e "1s/^/#define _inline inline\n/" posix-uefi/examples/0C_png/stb_image.h; \
	export PATH=$(BUILD_DIR_TOOLCHAIN)/bin/:$$PATH; USE_GCC=$(USE_GCC) make -C posix-uefi/uefi;

##########
# limine #
##########

limine-build:
	cd limine; \
	git checkout v2.0-branch-binary; \
	make;

#########
# Clean #
#########

clean:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_DIR)/posix-uefi
	mkdir $(BUILD_DIR)/uefi
	mkdir $(BUILD_DIR)/toolchain
