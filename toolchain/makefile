###########
# General #
###########
THREADS=8
TARGET=x86_64-elf
BUILD_DIR="$$(pwd)/../../build/"

.PHONY: gcc binutils remove-toolchain

build: binutils gcc

############
# binutils #
############

# ld, objdump...

binutils:
	git submodule update --init
	cd binutils-gdb; \
	mkdir build; \
	cd build; \
	../configure --target=$(TARGET) --prefix="$(BUILD_DIR)" --with-sysroot --disable-nls --disable-werror; \
	make -j$(THREADS); \
	make install;
	
# --disable-nls: Tells binutils not to include native language support.
# --disable-werror: Don't treat warnings as errors
# --with-sysroot: Tells binutils to enable sysroot support in the cross-compiler by pointing it to a default empty directory

########
# gcc  #
########

# gcc toolchain

gcc:
	#git submodule update --init
	#git checkout releases/gcc-11.1.0;
	cd gcc; \
	./contrib/download_prerequisites; \
	mkdir build; \
	cd build; \
	export PATH=$(BUILD_DIR)bin/:$$PATH; \
	../configure --target=x86_64-elf --prefix="$(BUILD_DIR)" --disable-nls --enable-languages=c,c++ --without-headers; \
	make -j$(THREADS) all-gcc; \
	make -j$(THREADS) all-target-libgcc; \
	make install-gcc; \
	make install-target-libgcc;
	
# --disable-nls: Disable other languages support (only English output)
# --enable-languages: Build support for c,c++ only
# --without-headers: Tells GCC not to rely on any C library (standard or runtime) present for the target


# Not "clean" to avoid mistakes
remove-toolchain:
	rm -rf $(BUILD_DIR)*
