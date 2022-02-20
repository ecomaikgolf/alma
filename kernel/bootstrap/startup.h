/**
 * Startup functions of the kernel
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "acpi/acpi.h"
#include "interrupts/interrupts.h"
#include "screen/fonts/psf1.h"
#include "screen/framebuffer.h"
#include "segmentation/gdt.h"
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

void screen(screen::framebuffer *, screen::fonts::psf1 *);
void allocator(uefi::memory::map *);
void gdt();
void translator(uefi::memory::map *);
void interrupts();
void enable_virtualaddr();
void enable_interrupts();
void keyboard();
void acpi(acpi::rsdp_v2 *);
void pci();
void heap(void *, size_t);

} // namespace bootstrap
