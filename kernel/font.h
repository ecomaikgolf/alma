/**
 * Font definitions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

/** X glyph size in pixels */
const int PSF1_X = 8;
/** Y glyph size in pixels */
const int PSF1_Y = 16;

/**
 * PSF1 Font Header
 */
struct PSF1_Header
{
    /** 0x36, 0x04 */
    unsigned char magic[2];
    /** number of glyphs mode */
    unsigned char mode;
    /** size of each glyph in the glyph buffer */
    unsigned char charsize;
};

/**
 * PSF1 Font
 */
struct PSF1_Font
{
    /** Header */
    PSF1_Header *header;
    /** glyph buffer */
    void *buffer;
};
