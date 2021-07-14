/**
 * Class to output to the screen
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "renderer.h"

/**
 * Renderer constructor
 *
 * @param fb Framebuffer (GOP wrapper to print)
 * @param font PSF1 bitmap font
 * @param x_offset X **pixel** initial position
 * @param y_offset Y **pixel** initial position
 * @param color hex color (enum)
 */
Renderer::Renderer(Framebuffer *fb,
                   PSF1_Font *font,
                   unsigned int x_offset,
                   unsigned int y_offset,
                   Color color)
{
    this->fb       = fb;
    this->font     = font;
    this->x_offset = x_offset;
    this->y_offset = y_offset;
    this->color    = color;
}

/**
 * Draw a character to the screen
 *
 * Doesn't increase x_offset and y_offset, also does not recognise special chars
 *
 * @param character Char to print
 */
void
Renderer::draw(const char character)
{
    /* Select character from glyph buffer (a character is composed by charsize
     * elements )*/
    char *chr = (char *)this->font->buffer + (character * this->font->header->charsize);

    /* Iterate bitmap "rectangle" (with offset as base) */
    for (unsigned long y = this->y_offset; y < this->y_offset + PSF1_Y; y++) {
        for (unsigned long x = this->x_offset; x < this->x_offset + PSF1_X; x++) {
            /* Each Y from bitmap is a "flag number", check if corresponding
             * x bit is set */
            if ((*chr & (0b10000000 >> (x - this->x_offset)))) {
                *(unsigned int *)(this->fb->base + x + (y * this->fb->ppscl)) =
                  (unsigned int)this->color;
            }
        }
        chr++; /* increase char to iterate over entire charsize elements */
    }
}

/**
 * Draw a character to the screen
 *
 * Increases x_ffset and y_offset, also recognises special chars as '\n'
 *
 * @param character Char to print
 */
void
Renderer::put(const char character)
{
    char addterm[2] = { character, '\0' };
    Renderer::print(addterm);
}

/**
 * Print a string without default newline
 *
 * Increases x_offset and y_offset, also recognises special chars as '\n'
 *
 * @param str string to print
 */
void
Renderer::print(const char *str)
{
    int i = 0;
    while (str[i]) {
        switch (str[i]) {
            case '\n':
                this->y_offset += 16;
                this->x_offset = 0;
                break;
            default: {
                Renderer::draw(str[i]);
                this->x_offset += PSF1_X;
                if (this->x_offset >= this->fb->width) {
                    this->y_offset += PSF1_Y;
                    this->x_offset = 0;
                }
            }
        }
        i++;
    }
}

/**
 * Print a string with a default newline
 *
 * Increases x_offset and y_offset, also recognises special chars as '\n'
 *
 * @param str string to print
 */
void
Renderer::println(const char *str)
{
    Renderer::print(str);
    Renderer::put('\n');
}

/**
 * Sets a new char color to use
 *
 * @param color Color to use onwards
 */
void
Renderer::setColor(Color color)
{
    this->color = color;
}
