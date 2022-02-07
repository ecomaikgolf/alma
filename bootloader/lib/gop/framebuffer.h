/**
 * Funtionalities to work with the framebuffer
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <uefi.h>

/**
 * Framebuffer struct
 */
typedef struct
{
    /** Base address of the framebuffer */
    char *base;
    /** Total size */
    unsigned long long buffer_size;
    /** Screen width */
    unsigned int width;
    /** Screen height */
    unsigned int height;

    /**
     * Pixels per scan line
     *
     * Usually equal to width but sometimes not due to extra padding beyond the
     * screen width. We must use it for linear array access (x + (y * ppscl))
     * instead of screen width
     */
    unsigned int ppscl;
} Framebuffer;

Framebuffer *create_fb(const efi_gop_t *const);
