/**
 * Managing fonts
 *
 * @info https://www.win.tue.nl/~aeb/linux/kbd/font-formats-1.html
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "io/file.h"
#include "log/stdout.h"

/**
 * PSF1 Header magic numbers
 */
#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

/**
 * PSF1 Header
 */
typedef struct
{
    /** Magic number */
    unsigned char magic[2];
    /** Mode to manage number of glpyhs */
    unsigned char mode;
    /** Size of a glyph bitmap */
    unsigned char charsize;
} PSF1_Header;

/**
 * PSF1 Font
 */
typedef struct
{
    /** PSF1 Header */
    PSF1_Header *header;
    /** Glyph buffer data */
    void *buffer;
} PSF1_Font;

PSF1_Font *load_psf1_font(const char *const);
PSF1_Header *get_psf1_header(const char *const);
void *get_psf1_glyph(const char *const);
uint8_t verify_psf1_header(const PSF1_Header *const);
