/**
 * Startup functions of the kernel
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "screen/fonts/psf1.h"
#include "screen/framebuffer.h"
#include "uefi/memory.h"

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

void allocator(uefi::memory::map *);

} // namespace bootstrap
