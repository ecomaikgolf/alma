/**
 * Framebuffer definition (same definition in bootloader/ )
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <stdint.h>

namespace screen {

/**
 * Framebuffer struct
 *
 * Wrapper of the GOP to print to the screen
 */
struct framebuffer
{
    unsigned int *base;
    unsigned long long buffer_size;
    unsigned int width;
    unsigned int height;
    unsigned int ppscl;

    framebuffer &operator=(framebuffer fb)
    {
        this->base        = fb.base;
        fb.base           = nullptr;
        this->buffer_size = fb.buffer_size;
        this->width       = fb.width;
        this->height      = fb.height;
        this->ppscl       = fb.ppscl;

        return *this;
    }
};

/**
 * Circular framebuffer struct
 *
 * Used to implement the double buffer method
 */
struct cache_framebuffer : framebuffer
{
    unsigned int *actual;
    unsigned int *limit;

    cache_framebuffer &operator=(framebuffer fb)
    {
        this->base        = nullptr;
        this->actual      = nullptr;
        this->limit       = nullptr;
        this->buffer_size = fb.buffer_size;
        this->width       = fb.width;
        this->height      = fb.height;
        this->ppscl       = fb.ppscl;

        return *this;
    }
};

} // namespace screen
