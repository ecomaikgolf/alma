/**
 * Startup functions of the kernel
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "acpi/acpi.h"
#include "interrupts/interrupts.h"
#include "screen/fonts/psf1.h"
#include "screen/framebuffer.h"
#include "segmentation/gdt.h"
#include "shell/command.h"
#include "stivale2.h"
#include "uefi/memory.h"
#include <stdint.h>

namespace bootstrap {

/**
 * Struct to be received from the bootloader
 */
struct boot_args
{
    screen::framebuffer *fb;
    screen::fonts::psf1 *font;
    uefi::memory::map *map;
    acpi::rsdp_v2 *rsdp;
};

void screen(stivale2_struct *);
void allocator(stivale2_struct *);
void gdt();
void translator(stivale2_struct *);
void interrupts();
void enable_virtualaddr();
void enable_interrupts();
void keyboard();
void acpi(stivale2_struct *);
void pci();
void heap(void *, size_t);

} // namespace bootstrap
