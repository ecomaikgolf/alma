/**
 * Kernel file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */
#include "stivale2.h"

static uint8_t stack[8192];

#include "acpi/acpi.h"
#include "bootstrap/startup.h"
#include "bootstrap/stivale_hdrs.h"
#include "float.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "io/keyboard.h"
#include "kernel.h"
#include "libc/stdlib.h"
#include "libc/string.h"
#include "paging/PFA.h"
#include "paging/PTM.h"
#include "pci/pci.h"
#include "screen/fonts/psf1.h"
#include "screen/framebuffer.h"
#include "screen/renderer.h"
#include "segmentation/gdt.h"
#include "shell/command.h"
#include <stddef.h>
#include <stdint.h>

/**
 *
 */

extern "C" void _init();
extern "C" void _fini();
/**
 * Kernel starting function
 * extern C to avoid C++ function mangling
 */
extern "C" [[noreturn]] void
_start(stivale2_struct *stivale2_struct)
{
    _init();

    stivale2_struct_tag_memmap *map =
      (stivale2_struct_tag_memmap *)stivale2_get_tag(stivale2_struct, 0x2187f79e8612de07);
    stivale2_struct_tag_framebuffer *fb =
      (stivale2_struct_tag_framebuffer *)stivale2_get_tag(stivale2_struct, 0x506461d2950408fa);
    stivale2_struct_tag_rsdp *rsdp =
      (stivale2_struct_tag_rsdp *)stivale2_get_tag(stivale2_struct, 0x9e1786930a375e78);
    stivale2_struct_tag_terminal *cmd =
      (stivale2_struct_tag_terminal *)stivale2_get_tag(stivale2_struct, 0xc2b3f4c3233b0974);
    stivale2_struct_tag_modules *mod =
      (stivale2_struct_tag_modules *)stivale2_get_tag(stivale2_struct, 0x4b6fe466aade04ce);
    stivale2_struct_tag_rsdp *rs =
      (stivale2_struct_tag_rsdp *)stivale2_get_tag(stivale2_struct, 0x9e1786930a375e78);

    screen::fonts::psf1 *font = nullptr;
    for (int i = 0; i < mod->module_count; i++) {
        if (strcmp(mod->modules[i].string, "font") == 0) {
            font = (screen::fonts::psf1 *)mod->modules[i].begin;
        }
    }

    screen::framebuffer frame;
    frame.base        = (unsigned int *)fb->framebuffer_addr;
    frame.buffer_size = fb->framebuffer_width * fb->framebuffer_pitch;
    frame.ppscl       = (fb->framebuffer_pitch / sizeof(uint32_t));
    frame.width       = fb->framebuffer_width;
    frame.height      = fb->framebuffer_height;

    screen::fonts::psf1 f;
    screen::fonts::psf1_header *f_hdr = (screen::fonts::psf1_header *)font;
    f.header                          = f_hdr;
    f.buffer                          = (uint8_t *)font + sizeof(screen::fonts::psf1_header);

    void (*stivale2_term_write)(uint64_t ptr, uint64_t length);

    stivale2_term_write = (void (*)(unsigned long, unsigned long))cmd->term_write;

    char aux[] = "hola";
    stivale2_term_write((uint64_t)&aux[0], 4);

    bootstrap::boot_args *args = nullptr;
    /* Bootstrap the kernel (function order is mandatory) */
    bootstrap::screen(&frame, &f);
    kernel::tty.println("hola");

    char mem[256];
    for (int i = 0; i < map->entries; i++) {
        if (map->memmap[i].type == 1) {
            hstr((uint64_t)map->memmap[i].base, mem);
            kernel::tty.print(mem);
            str((int)map->memmap[i].length / kernel::page_size, mem);
            kernel::tty.print("(");
            kernel::tty.print(mem);
            kernel::tty.print("), ");
            // str((int)map->memmap[i].length, mem);
            // kernel::tty.print(" - Lenght ");
            // kernel::tty.print(mem);
            // str((int)map->memmap[i].type, mem);
            // kernel::tty.print(" - Type ");
            // kernel::tty.print(mem);
            // kernel::tty.newline();
        }
    }

    bootstrap::allocator(map);

    kernel::tty.newline();

    bootstrap::translator(map);

    hstr((uint64_t)kernel::allocator.request_page(), mem);
    kernel::tty.println(mem);
    hstr((uint64_t)kernel::allocator.request_page(), mem);
    kernel::tty.println(mem);
    hstr((uint64_t)kernel::allocator.request_page(), mem);
    kernel::tty.println(mem);
    hstr((uint64_t)kernel::allocator.request_page(), mem);
    kernel::tty.println(mem);
    hstr((uint64_t)kernel::allocator.request_page(), mem);
    kernel::tty.println(mem);

    // bootstrap::screen(&frame, font);
    bootstrap::gdt();
    bootstrap::interrupts();
    // bootstrap::enable_virtualaddr();
    bootstrap::enable_interrupts();
    bootstrap::keyboard();
    bootstrap::acpi((acpi::rsdp_v2 *)rs->rsdp);
    // bootstrap::heap((void *)0x0000100000000000, 0x1000);

    bootstrap::pci();

    // kernel::tty.println("Hola desde el kernel!");
    // asm("int $0x09");
    // kernel::tty.println("Hola otra vez desde el kernel!");

    shell::commands::shell(0, nullptr);

    _fini();
    /* Shoudln't return */
    __asm__("hlt");
    /* To supress diagnostics */
    while (1) {
    };
}
