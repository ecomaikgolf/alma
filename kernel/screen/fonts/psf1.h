/**
 * Font definitions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "../colors.h"
#include "../framebuffer.h"
#include "../renderer.h"

namespace screen {

namespace fonts {

/**
 * PSF1 Font
 */
struct psf1
{
    /**
     * PSF1 Font Header
     */
    struct __attribute__((packed))
    {
        /** 0x36, 0x04 */
        unsigned char magic[2];
        /** number of glyphs mode */
        unsigned char mode;
        /** size of each glyph in the glyph buffer */
        unsigned char charsize;
    } * header;

    /** glyph buffer */
    void *buffer;

    /** Glyph x size in pixels */
    static const unsigned int glyph_x = 8;
    /** Glyph y size in pixels */
    static const unsigned int glyph_y = 16;
} __attribute__((packed));

} // namespace fonts

/**
 * Renderer with PSF1 fonts
 */
class psf1_renderer : public renderer_i
{
  public:
    psf1_renderer(framebuffer *,
                  fonts::psf1 *,
                  unsigned int x_offset = 0,
                  unsigned int y_offset = 0,
                  color_e color         = color_e::WHITE);
    psf1_renderer()        = default;
    psf1_renderer &operator=(psf1_renderer &&);

    void draw(const char);

  private:
    ///** renderer glyph x size */
    unsigned int glyph_x()
    {
        return fonts::psf1::glyph_x;
    }
    ///** renderer glyph y size */
    unsigned int glyph_y()
    {
        return fonts::psf1::glyph_y;
    }
    /** PSF1 font to use */
    fonts::psf1 *font;
};

} // namespace screen
