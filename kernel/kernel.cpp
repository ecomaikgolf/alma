/**
 * Kernel file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "kernel.h"
#include "acpi/acpi.h"
#include "bootstrap/startup.h"
#include "float.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "io/keyboard.h"
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
#include "stivale2.h"
#include <stddef.h>
#include <stdint.h>

static uint8_t stack[8192];

/**
 * Stivale configuration parameters
 */

static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag   = { .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID, .next = 0 },
    .flags = 0
};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag                = { .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
                            .next       = (uint64_t)&terminal_hdr_tag },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used)) static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack       = (uintptr_t)stack + sizeof(stack),
    .flags       = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    .tags        = (uintptr_t)&framebuffer_hdr_tag
};

extern "C" void _init();
extern "C" void _fini();

/**
 * Kernel starting function
 * extern C to avoid C++ function mangling
 */
extern "C" [[noreturn]] void
_start(stivale2_struct *stivale2_struct)
{
    /* Call global constructors & functions */
    _init();

    bootstrap::allocator(stivale2_struct);
    bootstrap::translator(stivale2_struct);
    bootstrap::screen(stivale2_struct);
    bootstrap::gdt();
    bootstrap::interrupts();
    // bootstrap::enable_virtualaddr();
    bootstrap::enable_interrupts();
    bootstrap::keyboard();
    bootstrap::acpi(stivale2_struct);
    //  bootstrap::heap((void *)0x0000100000000000, 0x1000);
    bootstrap::pci();

    kernel::tty.println("welcome to the alma kernel");

    shell::commands::shell(0, nullptr);

    /* Call global destructors & functions */
    _fini();
    /* Shoudln't return (poweroff) */
    __asm__("hlt");
    /* To supress diagnostics */
    while (1) {
    };
}
