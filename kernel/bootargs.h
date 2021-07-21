/**
 * Argument struct to be received from the bootloader
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "font.h"
#include "framebuffer.h"
#include "uefimmap.h"

/**
 * Struct to be received from the bootloader
 */
struct BootArgs
{
    Framebuffer *fb;
    PSF1_Font *font;
    MapInfo *map;
};
