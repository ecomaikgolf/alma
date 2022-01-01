/**
 * PSF1 font definition
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "psf1.h"

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
psf1_renderer::psf1_renderer(framebuffer *fb,
                             fonts::psf1 *font,
                             unsigned int x_offset,
                             unsigned int y_offset,
                             color_e color)
{
    this->fb       = fb;
    this->font     = font;
    this->x_offset = x_offset;
    this->y_offset = y_offset;
    this->color    = color;
}

psf1_renderer &
psf1_renderer::operator=(psf1_renderer &&mov)
{
    this->font     = mov.font;
    mov.font       = nullptr;
    this->fb       = mov.fb;
    mov.fb         = nullptr;
    this->x_offset = mov.x_offset;
    this->y_offset = mov.y_offset;
    this->color    = mov.color;

    return *this;
}

/**
 * Draw a character to the screen
 *
 * Doesn't increase x_offset and y_offset, also does not recognise special chars
 *
 * Overrided from renderer interface
 *
 * @param character Char to print
 */
void
psf1_renderer::draw(const char character)
{
    /* Select character from glyph buffer (a character is composed by charsize
     * elements )*/
    char *chr = static_cast<char *>(this->font->buffer) +
                (static_cast<unsigned char>(character) * this->font->header->charsize);

    /* Iterate bitmap "rectangle" (with offset as base) */
    for (unsigned long y = this->y_offset; y < this->y_offset + this->glyph_y(); y++) {
        for (unsigned long x = this->x_offset; x < this->x_offset + this->glyph_x(); x++) {
            /* Each Y from bitmap is a "flag number", check if corresponding
             * x bit is set */
            if ((*chr & (0b10000000 >> (x - this->x_offset)))) {
                *(static_cast<unsigned int *>(this->fb->base + x + (y * this->fb->ppscl))) =
                  static_cast<unsigned int>(this->color);
            }
        }
        chr++; /* increase char to iterate over entire charsize elements */
    }
}

} // namespace screen
