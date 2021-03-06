/**
 * Struct to be passed to the kernel
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "acpi/acpi.h"
#include "gop/font.h"
#include "gop/framebuffer.h"
#include "memory/memory.h"
#include <uefi.h>

typedef struct __attribute__((packed))
{
    Framebuffer *fb;
    PSF1_Font *font;
    MapInfo *map;
    rsdp_v2 *rsdp;
} BootArgs;
