/**
 * Kernel file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "kernel.h"
#include "acpi/acpi.h"
#include "bootstrap/startup.h"
#include "bootstrap/stivale_hdrs.h"
#include "float.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "io/keyboard.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "paging/BPFA.h"
#include "paging/PFA.h"
#include "paging/PTM.h"
#include "pci/pci.h"
#include "screen/fonts/psf1.h"
#include "screen/framebuffer.h"
#include "segmentation/gdt.h"
#include "shell/command.h"
#include "stivale2.h"
#include <stddef.h>
#include <stdint.h>

/** kernel stack */
static uint8_t stack[8192];

/** Stivale tag parameters */
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag   = { .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID, .next = 0 },
    .flags = 0
};

/** Stivale framebuffer parameters */
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = { .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
             .next       = (uint64_t)&terminal_hdr_tag },
    /* Best effort pick */
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

/** Stivale header stored in .stivale2hdr and readed by the bootloader */
__attribute__((section(".stivale2hdr"), used)) static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    /* Pointer to the end of the stack as it grows downwards */
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    .tags  = (uintptr_t)&framebuffer_hdr_tag
};

/** Call global constructors & destructors (generated by gcc) */
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

    /* Store the stivale header pointer */
    kernel::internal::stivalehdr = *stivale2_struct;

    /* Bootstrap the kernel (order is important) */
    bootstrap::allocator(stivale2_struct);
    bootstrap::translator(stivale2_struct);
    bootstrap::enable_virtualaddr();
    bootstrap::heap(0x10);
    bootstrap::screen(stivale2_struct);
    bootstrap::gdt();
    bootstrap::interrupts();
    bootstrap::enable_interrupts();
    bootstrap::keyboard();
    bootstrap::acpi(stivale2_struct);
    bootstrap::pci();
    bootstrap::rtl8139();

    /* Welcome the user */
    kernel::tty.println("welcome to the alma kernel");

    /* Start a shell */
    shell::commands::shell(0, nullptr);

    /* Call global destructors & functions */
    _fini();
    /* Shoudln't return (poweroff instead) */
    __asm__("hlt");
    /* To supress diagnostics */
    while (1) {
    };
}
