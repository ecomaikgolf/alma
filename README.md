<p align="center">
  <img width="300" height="165" src="/docs/img/alma_logo.png">
</p>

<p align="center">
  <i>alma is a toy kernel written in C++ for x86_64 machines with the mere purpose of learning OS development</i>
</p>

# alma

> 6. f. Sustancia o parte principal de cualquier cosa.

## Features

_Note: GIF quality is vastly reduced and native executions look clear_

<table>
   <tr>
      <th>alma build system</th>
      <th></th>
   </tr>
   <tbody>
      <tr>
         <td>
            <ul>
               <li>
                  Setup toolchain
                  <ul>
                     <li>apt install ...</li>
                     <li>make -C toolchain</li>
                     <li>alma build vm</li>
                  </ul>
               </li>
               <li>
                  Configure with cmake
                  <ul>
                     <li>Out of source builds</li>
                     <li>Recompile only what is needed</li>
                  </ul>
               </li>
               <li>
                  Build with make or ninja
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
            "/docs/img/alma_build.gif"></td>
      </tr>
   </tbody>
</table>
<table>
   <tr>
      <th>alma kernel</th>
      <th></th>
   </tr>
   <tbody>
      <tr>
         <td>
            <ul>
               <li>Tested in real hardware</li>
               <li>Stivale2 specification </li>
               <li>Call ctors and dtors</li>
               <li>
                  Rendering 
                  <ul>
                     <li>simple rendering</li>
                     <li>fast console rendering</li>
                     <li>PSF1 fonts</li>
                  </ul>
               </li>
               <li>
                  Segmentation
                  <ul>
                     <li>Naive GDT</li>
                  </ul>
               </li>
               <li>
                  Paging
                  <ul>
                     <li>Virtual Memory</li>
                     <li>Page Allocation</li>
                  </ul>
               </li>
               <li>
                  Keyboard
                  <ul>
                     <li>PS2 Keyboard Support  </li>
                     <li>symbols & mayus</li>
                  </ul>
               </li>
               <li>
                  Networking
                  <ul>
                     <li>RTL8139 Driver</li>
                  </ul>
               </li>
               <li>
                  ACPI
                  <ul>
                     <li>Query Tables</li>
                  </ul>
               </li>
               <li>
                  PCI
                  <ul>
                     <li>MCFG From ACPI</li>
                     <li>MMIO Configuration</li>
                     <li>Enumerate dev/bus/func </li>
                  </ul>
               </li>
               <li>
                  Interrupts
                  <ul>
                     <li>Manage IDT</li>
                     <li>Register Interrupts</li>
                  </ul>
               </li>
               <li>
                  Dynamic Memory
                  <ul>
                     <li>malloc</li>
                     <li>free</li>
                  </ul>
               </li>
               <li>WORK IN PROGRESS &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp; &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;</li>
            </ul>
         </td>
         <td>
            <img src="/docs/img/alma_virtmem.gif">
            <img src="/docs/img/alma_net.gif">
         </td>
      </tr>
   </tbody>
</table>
<table>
   <tr>
      <th>alma bootloader</th>
      <th></th>
   </tr>
   <tbody>
      <tr>
         <td>
            <ul>
               <li>
                  Deprecated
                  <ul>
                     <li>alma uses stivale2</li>
                     <li>ships with the limine botloader &nbsp;</li>
                  </ul>
               </li>
               <li>posix-uefi</li>
               <li>
                  Load ELF
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
            "/docs/img/alma_bootloader.gif"></td>
      </tr>
   </tbody>
</table>

## Run

1. Install qemu:

  **Ubuntu**:
```bash
apt install qemu-system-x86 qemu-system-gui
```

  **Arch Linux**:
```bash
pacman -S qemu qemu-arch-extra
```

2. Find a build to test https://ls.ecomaikgolf.com/alma/builds/

3. Download a build with wget:

```bash
wget https://ls.ecomaikgolf.com/alma/builds/CHANGEME.tar.gz
tar xf CHANGEME.tar.gz
cd CHANGEME
```

4. Run

```bash
qemu-system-x86_64 -machine q35 -cpu qemu64 -m 256M -bios bios.bin -netdev user,id=user.0 -device rtl8139,netdev=user.0,mac=ca:fe:c0:ff:ee:00 -object filter-dump,id=f1,netdev=user.0,file=log.pcap -boot d -cdrom alma.iso
```

If you want to run it in real hardware, flash it to a USB device:

```bash
sudo dd bs=4M if=alma.iso of=/dev/CHANGEME conv=fdatasync status=progress
```

Remember that you must have certain hardware for it to work (PS2 keyboard, RTL8139 NIC, PCIe...). Even with that, running in real hardware is a dark dark place... things can go wrong.

## Build

### Virtual Machine (Method 1)

1. Download the alma build vm [.ova](https://drive.google.com/file/d/1ilYIGXWLiMP9ULE48gVu7FeTnDZqmqmw/view?usp=sharing) (a 6.31GB Xubuntu 20.04 virtualbox VM ready to compile)

<p align="center">
  <img width="800" height="400" src="/docs/img/alma_vm.png">
</p>

2. Compile with the "Build Alma" icon
3. Run with the "Run Alma" icon
3. Update (pull) with the "Update Alma" icon

### Manual (Method 2)

_Note: Instructions tested on `Ubuntu 20.04`_

1. Install dependencies

```bash
apt install nasm iasl cmake make qemu-system-x86 qemu-system-gui git uuid-dev python python3-distutils bash texinfo bison flex build-essential mtools
```

2. `make -C toolchain/`

* EDKII build can be manually disabled if a compiled binary is provided or not using UEFI
* posix-uefi build can be manually disabled if bootloader is not going to be built (default behaviour)

3. `cmake -B build` or `cmake -B build -GNinja` 
4. `make -C build run` or `ninja -C build run` 

## Mirrors

* Github: `git@github.com:ecomaikgolf/alma.git`, `https://github.com/ecomaikgolf/alma.git`
* Gitlab: `git@gitlab.com:ecomaikgolf/os-dev.git`, `https://gitlab.com/ecomaikgolf/os-dev.git`
* Selfhost: `git@ecomaikgolf.com:alma.git`

## Bugs

Please explain them to me: [me 𝒂𝒕 ecomaikgolf ꓒσ𝗍 c0m](https://ecomaikgolf.com/contact.html) 

Take into acccount this is a Bachelor Final Project and has been developed under the pressure of a deadline among other subjects. Some things could be improved, for example, with better (and more complex) data structures, but it's not the goal of the project.

## FAQ

> Q: Muh tons of dependencies
> 
> A: I'm compiling binutils, gdb, edk2 and posix-uefi from source

> Q: 6.31GB VM
>
> A: I can't do more. Xubuntu + "zeroed" free memory before exporting so it can be compressed.

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
* [me 𝒂𝒕 ecomaikgolf ꓒσ𝗍 c0m](https://ecomaikgolf.com/contact.html) `C79F 01CE 017F 57A4 FBBB 4E22 33DD FB0A EB94 20CB` 
* https://ecomaikgolf.com/
* https://twitter.com/ecomaikgolf
