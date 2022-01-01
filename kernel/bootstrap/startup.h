/**
 * Startup functions of the kernel
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

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
};

void screen(screen::framebuffer *, screen::fonts::psf1 *);
void allocator(uefi::memory::map *);
void gdt();

} // namespace bootstrap
