/**
 * Argument struct to be received from the bootloader
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "screen/fonts/psf1.h"
#include "screen/framebuffer.h"
#include "uefi/memory_map.h"

namespace bootstrap {

/**
 * Struct to be received from the bootloader
 */
struct bootloader_args
{
    screen::framebuffer *fb;
    screen::fonts::psf1 *font;
    UEFIMMap::Map *map;
};

} // namespace bootstrap
