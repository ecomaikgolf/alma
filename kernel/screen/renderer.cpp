/**
 * Class to output to the screen
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "screen/renderer.h"
#include "colors.h"

// screen::renderer_i *global;

namespace screen {

/**
 * Draw a character to the screen
 *
 * Increases x_ffset and y_offset, also recognises special chars as '\n'
 *
 * @param character Char to print
 */
void
renderer_i::put(const char character)
{
    char aux[2] = { character, '\0' };
    this->print(aux);
}

/**
 * Print a string without default newline
 *
 * Increases x_offset and y_offset, also recognises special chars as '\n'
 *
 * @param str string to print
 */
void
renderer_i::print(const char *str)
{
    int i = 0;
    while (str[i]) {
        switch (str[i]) {
            case '\n':
                this->y_offset += this->glyph_y();
                this->x_offset = 0;
                break;
            default: {
                this->draw(str[i]);
                this->x_offset += this->glyph_x();
                if (this->x_offset >= this->fb->width)
                    this->print("\n");
            }
        }
        if ((this->y_offset + this->glyph_y()) > this->fb->height)
            this->scroll();
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
renderer_i::println(const char *str)
{
    this->print(str);
    this->put('\n');
}

/**
 * Sets a new char color to use
 *
 * @param color Color to use onwards
 */
void
renderer_i::setColor(color_e color)
{
    this->color = color;
}

/**
 * Fast screen clear
 *
 * Uses biggest integer asignations
 */
void
renderer_i::clear()
{
    uint32_t jumps = fb->buffer_size / sizeof(uint64_t);
    uint32_t rest  = fb->buffer_size % sizeof(uint64_t);

    /* Fast clear, use big integer movements */
    unsigned int i;
    for (i = 0; i < jumps; i++)
        *((uint64_t *)fb->base + i) = static_cast<uint8_t>(screen::color_e::BLACK);

    /* for sizes < 64 bytes */
    for (unsigned int j = 0; j < rest; j++)
        *((uint8_t *)((uint64_t *)fb->base + i) + j) = static_cast<uint8_t>(screen::color_e::BLACK);

    this->x_offset = 0;
    this->y_offset = 0;
}

void
renderer_i::scroll()
{
    uint32_t mrg = fb->ppscl * (this->glyph_y() * 4);
    uint32_t dif = fb->buffer_size - mrg;
    uint8_t *src = (uint8_t *)fb->base + mrg;
    uint8_t *dst = (uint8_t *)fb->base;

    {
        uint32_t jumps = dif / sizeof(uint64_t);
        uint32_t rest  = dif % sizeof(uint64_t);

        unsigned int i;
        for (i = 0; i < jumps; i++)
            *((uint64_t *)dst + i) = *((uint64_t *)src + i);

        for (unsigned int j = 0; j < rest; j++)
            *((uint8_t *)((uint64_t *)dst + i) + j) = *((uint8_t *)((uint64_t *)src + i) + j);
    }

    {
        uint8_t *clr = ((uint8_t *)fb->base + fb->buffer_size) - mrg;

        uint32_t jumps = mrg / sizeof(uint64_t);
        uint32_t rest  = mrg % sizeof(uint64_t);

        unsigned int i;
        for (i = 0; i < jumps; i++)
            *((uint64_t *)clr + i) = 0;

        for (unsigned int j = 0; j < rest; j++)
            *((uint8_t *)((uint64_t *)clr + i) + j) = 0;
    }

    if (this->y_offset >= this->glyph_y())
        this->y_offset -= this->glyph_y();
}

} // namespace screen
