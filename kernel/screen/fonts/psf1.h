#pragma once

namespace screen {

namespace fonts {

struct psf1_header
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
struct psf1
{
    /**
     * PSF1 Font Header
     */
    psf1_header header;

    /** glyph buffer */
    void *buffer;

    /** Glyph x size in pixels */
    static const unsigned int glyph_x = 8;
    /** Glyph y size in pixels */
    static const unsigned int glyph_y = 16;
};

} // namespace fonts

} // namespace screen
