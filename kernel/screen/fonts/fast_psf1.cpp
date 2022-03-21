/**
 * Fast PSF1 rendering
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "screen/fonts/fast_psf1.h"
#include "lib/math.h"

namespace screen {

/**
 * Renderer constructor
 *
 * @param fb Framebuffer (GOP wrapper to print)
 * @param font PSF1 bitmap font
 * @param x_offset X **pixel** initial position
 * @param y_offset Y **pixel** initial position
 * @param color hex color (enum)
 */
fast_psf1::fast_psf1(framebuffer video_memory,
                     fonts::psf1 font,
                     unsigned int x_offset,
                     unsigned int y_offset,
                     color_e color)
  : fast_renderer_i(video_memory, x_offset, y_offset, color)
  , font(font)
{}

fast_psf1 &
fast_psf1::operator=(fast_psf1 &&mov)
{
    this->font            = mov.font;
    this->video_memory    = mov.video_memory;
    this->video_cache     = mov.video_cache;
    mov.video_memory.base = nullptr;
    mov.video_cache.base  = nullptr;
    this->x_offset        = mov.x_offset;
    this->y_offset        = mov.y_offset;
    this->color           = mov.color;

    return *this;
}

/**
 * Draw a character to the screen
 *
 * Doesn't increase x_offset and y_offset, also does not recognise special chars
 *
 * Overrided from renderer interface
 *
 * @warning THIS DOESN'T UPDATE VIDEO MEMORY, JUST CACHE MEMORY
 *
 * @param character Char to print
 */
void
fast_psf1::draw(const char character)
{
    /* Select character from glyph buffer (a character is composed by charsize
     * elements )*/
    char *chr = static_cast<char *>(this->font.buffer) +
                (static_cast<unsigned char>(character) * this->font.header.charsize);

    /* Iterate bitmap "rectangle" (with offset as base) */
    for (uint32_t y = this->y_offset; y < this->y_offset + this->glyph_y(); y++) {
        for (uint32_t x = this->x_offset; x < this->x_offset + this->glyph_x(); x++) {
            /* Each Y from bitmap is a "flag number", check if corresponding
             * x bit is set */
            if ((*chr & (0b10000000 >> (x - this->x_offset))))
                this->draw_pixel(x, y);
        }
        chr++; /* increase char to iterate over entire charsize elements */
    }
}

} // namespace screen
