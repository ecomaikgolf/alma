/**
 * Font definitions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "screen/colors.h"
#include "screen/framebuffer.h"

namespace screen {

namespace fonts {

namespace specification {
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
} // namespace specification

/**
 * Renderer with PSF1 fonts
 */
template<typename T>
class psf1 : public T
{
  public:
    psf1(screen::framebuffer fb,
         screen::fonts::specification::psf1 font,
         unsigned int x_offset = 0,
         unsigned int y_offset = 0,
         color_e color         = color_e::WHITE)
      : T(fb, x_offset, y_offset, color)
      , font(font)
    {}

    psf1() = default;

    psf1 &operator=(psf1 &&rhs)
    {
        T::operator=(rhs);
        this->font = rhs.font;
        return *this;
    }

    void draw(const char character)
    {
        /* Select character from glyph buffer (a character is composed by charsize
         * elements )*/
        char *chr = static_cast<char *>(this->font.buffer) +
                    (static_cast<unsigned char>(character) * this->font.header.charsize);

        /* Iterate bitmap "rectangle" (with offset as base) */
        for (unsigned long y = this->y_offset; y < this->y_offset + this->glyph_y(); y++) {
            for (unsigned long x = this->x_offset; x < this->x_offset + this->glyph_x(); x++) {
                /* Each Y from bitmap is a "flag number", check if corresponding
                 * x bit is set */
                if ((*chr & (0b10000000 >> (x - this->x_offset))))
                    this->draw_pixel(x, y);
            }
            chr++; /* increase char to iterate over entire charsize elements */
        }
    }

    ///** renderer glyph x size */
    unsigned int glyph_x()
    {
        return fonts::specification::psf1::glyph_x;
    }
    ///** renderer glyph y size */
    unsigned int glyph_y()
    {
        return fonts::specification::psf1::glyph_y;
    }

  private:
    /** PSF1 font to use */
    fonts::specification::psf1 font;
};

} // namespace fonts
} // namespace screen
