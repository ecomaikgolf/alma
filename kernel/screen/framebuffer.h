/**
 * Framebuffer definition (same definition in bootloader/ )
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

/**
 * Framebuffer struct
 *
 * Wrapper of the GOP to print to the screen
 */
struct Framebuffer
{
    unsigned int *base;
    unsigned long long buffer_size;
    unsigned int width;
    unsigned int height;
    unsigned int ppscl;
};
