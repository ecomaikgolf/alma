<p align="center">
  <img width="300" height="100" src="https://ls.ecomaikgolf.com/alma/img/logo3.png">
</p>

<p align="center">
  <i>alma is a toy kernel written in C++ for x86_64 machines with the mere purpose of learning OS development</i>
</p>

# alma

> 6. f. Sustancia o parte principal de cualquier cosa.

## Features

  <table>
    <tr>
      <th>alma build system</th>
    </tr>
    <tbody>
      <tr>
        <td>
          <ul>
            <li>Setup toolchain
              <ul>
                <li>apt install ...</li>
                <li>make -C toolchain</li>
                <li>alma build vm</li>
              </ul>
            </li>
            <li>Configure with cmake
              <ul>
                <li>Out of source builds</li>
                <li>Recompile only what is needed</li>
              </ul>
            </li>
            <li>Build with make or ninja
              <ul>
                <li>default target (build iso)</li>
                <li>target run (qemu)</li>
                <li>target debug (remote gdb)</li>
                <li>target doc (doxygen)</li>
              </ul>
            </li>
            <li>Speedup with ccache if found</li>
          </ul>
        </td>
        <td><img src=
        "https://ls.ecomaikgolf.com/alma/img/alma_build.gif"></td>
      </tr>
    </tbody>
  </table>
  <table>
    <tr>
      <th>alma bootloader</th>
    </tr>
    <tbody>
      <tr>
        <td>
          <ul>
            <li>Deprecated
              <ul>
                <li>alma uses stivale2</li>
                <li>ships with the limine botloader &nbsp;</li>
              </ul>
            </li>
            <li>posix-uefi</li>
            <li>Load ELF
              <ul>
                <li>Parse headers</li>
                <li>Load segments</li>
              </ul>
            </li>
            <li>Initialization tasks</li>
            <ul>
              <li>GOP</li>
              <li>Memory Map</li>
              <li>RSDP</li>
              <li>PSF1</li>
            </ul>
            <li>Kernel launch</li>
            <ul>
              <li>Call ctors</li>
              <li>Exit UEFI Services</li>
              <li>Jump to entry point</li>
            </ul>
          </ul>
        </td>
        <td><img src=
        "https://ls.ecomaikgolf.com/alma/img/alma_bootloader.gif"></td>
      </tr>
    </tbody>
  </table>
</body>
</html>






| <div style="width:160px">alma bootloader</div> |  |
|---------|-----------------------------------------------------|
| <ul><li>Deprecated<ul><li>Now alma follows the stivale2 protocol</li><li>builds with limine/li></ul></li><li></li></ul> | ![](https://ls.ecomaikgolf.com/alma/img/alma_psf1.gif) |



| Feature |  |
|---------|-----------------------------------------------------|
| PSF1 Font Rendering and Colors. println() and fmt(...) functions | ![](https://ls.ecomaikgolf.com/alma/img/alma_psf1.gif) |
| PS2 Keyboard with mayus, symbols, delete, etc. scanf() function | ![](https://ls.ecomaikgolf.com/alma/img/alma_scanf.gif) |
| Terminal Interface with scrolling | ![](https://ls.ecomaikgolf.com/alma/img/alma_scroll.gif) |
| RTL8139 Network Chip Driver | ![](https://ls.ecomaikgolf.com/alma/img/alma_net.gif) |
| Virtual Memory and Page Allocator | ![](https://ls.ecomaikgolf.com/alma/img/alma_virtmem.gif) |
| Loading | ![](https://ls.ecomaikgolf.com/alma/img/screen.png) |
| Loading | ![](https://ls.ecomaikgolf.com/alma/img/screen.png) |
| Loading | ![](https://ls.ecomaikgolf.com/alma/img/screen.png) |



## Run

0. Install dependencies (listed below)
1. Find build to test https://ls.ecomaikgolf.com/alma/builds/
2. Download a build with wget:

```bash
wget https://ls.ecomaikgolf.com/alma/builds/20211012-e906c4c.tar.gz # <-- Change build version!
tar xf 20211012-e906c4c.tar.gz
cd 20211012-e906c4c
```

3. Download precompiled edk2 Ovmf UEFI

```bash
wget https://ls.ecomaikgolf.com/alma/builds/bios.bin
```

4. Run

```bash
make
```

### Run Dependencies

* `qemu-system-x86`

Ubuntu: `apt install qemu-system-x86 qemu-system-gui`

Arch-Linux: `pacman -S qemu qemu-arch-extra`

## Build

### Virtual Machine

Get the "Alma Build VM" (.ova 5.9G), a Xubuntu 20.04 virtualbox VM ready to 
compile.

**Download (@gcloud.ua.es):** https://drive.google.com/file/d/1neuuBo7Ja4Czmwre1SWPkwGSF8MV98s3/view?usp=sharing

<p align="center">
  <img width="800" height="400" src="https://ls.ecomaikgolf.com/alma/img/alma-build-vm.png">
</p>

Features:
* already compiled toolchain
* systemwide ccache
* preconfigured "neutral" IDE (vscodium)
* one click functionalities
	* compile
	* run
	* update
	* clean

### Manual

Tested on `Ubuntu 20.04` 

1. Install dependencies (listed below)
2. `make -C toolchain/`
3. `cmake -B build`
4. `make -C build run`

#### Build Dependencies

* nasm
* iasl
* cmake
* qemu-system-x86
* qemu-system-gui
* uuid-dev
* python
* python3-distutils
* texinfo
* bison
* flex
* mtools

Remember to have: `make` `git` `bash` `build-essential`

Ubuntu 20.04 (and similars) one-command dependencies installer:

```bash
apt install nasm iasl cmake make qemu-system-x86 qemu-system-gui git uuid-dev python python3-distutils bash texinfo bison flex build-essential mtools
```

## Bugs

Please explain them to me: `me@ecomaikgolf.com`

## FAQ

> Q: Muh tons of dependencies
> 
> A: I have to compile binutils, gdb, edk2 and posix-uefi from source

> Q: 5.9GB VM
>
> A: I can't do more. Xubuntu + "zeroed" free memory before exporting so it can compress it.

> Q: Frequently asked questions?
>
> A: More like preanswered questions

## Bibliography

* https://wiki.osdev.org/Main_Page
* https://youtu.be/NpcGMuI7hxk
* https://0xax.gitbooks.io/linux-insides/content/Interrupts/
* https://youtu.be/mpPbKEeWIHU
* https://www.iaik.tugraz.at/teaching/materials/os/tutorials/paging-on-intel-x86-64/
* https://git.musl-libc.org/cgit/musl/tree
* https://maskray.me/blog/2021-11-07-init-ctors-init-array
* https://blog.llandsmeer.com/tech/2019/07/21/uefi-x64-userland.html

_The list is longer but I can't put each site where I read something. This list is a good place to start_

## Author

Ernesto Martínez García
* me@ecomaikgolf.com _(C79F 01CE 017F 57A4 FBBB 4E22 33DD FB0A EB94 20CB)_
* emg162@alu.ua.es 
  [[verify](https://github.com/leereilly/swot/blob/master/lib/domains/es/ua.txt)]
* https://ecomaikgolf.com/
* https://twitter.com/ecomaikgolf
