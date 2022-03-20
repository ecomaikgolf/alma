/**
 * PSF1 font definition
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "screen/fonts/fast_psf1.h"
#include "kernel.h"
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
fast_psf1_renderer::fast_psf1_renderer(framebuffer video_memory,
                                       fonts::psf1 font,
                                       unsigned int x_offset,
                                       unsigned int y_offset,
                                       color_e color)
{
    this->video_memory = video_memory;

    // TEMPORARY HOTFIX (until I fix malloc :( ) works on my machine^TM :D
    static uint32_t arr[480000];

    /* Create the cache buffer */
    this->video_cache        = video_memory;
    this->video_cache.base   = &arr[0];
    this->video_cache.actual = this->video_cache.base;
    this->video_cache.limit =
      (unsigned int *)((uint8_t *)this->video_cache.base + this->video_memory.buffer_size);

    this->font     = font;
    this->x_offset = x_offset;
    this->y_offset = y_offset;
    this->color    = color;

    uint64_t steps     = this->video_memory.buffer_size / sizeof(uint64_t);
    uint32_t remainder = this->video_memory.buffer_size % sizeof(uint64_t);

    for (uint64_t i = 0; i < steps; i++)
        ((uint64_t *)this->video_cache.base)[i] = (int)screen::color_e::FUCHSIA;

    for (uint32_t i = 0; i < remainder; i++)
        ((uint8_t *)this->video_cache.base)[i] = (int)screen::color_e::FUCHSIA;

    this->update_video();
}

fast_psf1_renderer &
fast_psf1_renderer::operator=(fast_psf1_renderer &&mov)
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
fast_psf1_renderer::draw(const char character)
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
            if ((*chr & (0b10000000 >> (x - this->x_offset)))) {

                *(static_cast<unsigned int *>(this->video_cache.base + x +
                                              (y * this->video_cache.ppscl))) =
                  static_cast<unsigned int>(this->color);

                *(static_cast<unsigned int *>(this->video_memory.base + x +
                                              (y * this->video_memory.ppscl))) =
                  static_cast<unsigned int>(this->color);
            }
        }
        chr++; /* increase char to iterate over entire charsize elements */
    }
}

} // namespace screen
