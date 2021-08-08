/**
 * Argument struct to be received from the bootloader
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "screen/font.h"
#include "screen/framebuffer.h"
#include "uefi/memory_map.h"

/**
 * Struct to be received from the bootloader
 */
struct BootArgs
{
    Framebuffer *fb;
    PSF1_Font *font;
    UEFIMMap::Map *map;
};
