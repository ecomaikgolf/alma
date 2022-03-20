/**
 * Class to output to the screen
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "screen/fast_renderer.h"
#include "colors.h"
#include "lib/math.h"
#include "lib/stdlib.h"

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
fast_renderer_i::put(const char character)
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
fast_renderer_i::print(const char *str, int64_t n)
{
    int i = 0;
    while (str[i] && n != 0) {
        switch (str[i]) {
            case '\n':
                this->y_offset += this->glyph_y();
                this->x_offset = 0;
                break;
            default: {
                this->draw(str[i]);
                this->x_offset += this->glyph_x();
                if (this->x_offset >= this->video_cache.width)
                    this->print("\n");
            }
        }
        if ((this->y_offset + this->glyph_y()) > this->video_cache.height)
            this->scroll();

        n--;
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
fast_renderer_i::println(const char *str)
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
fast_renderer_i::setColor(color_e color)
{
    this->color = color;
}

/**
 * Gets current color
 *
 * @return color Color in use
 */
color_e
fast_renderer_i::getColor()
{
    return this->color;
}

/**
 * Fast screen clear
 *
 * Uses biggest integer asignations
 */
void
fast_renderer_i::clear()
{
    uint32_t jumps = this->video_cache.buffer_size / sizeof(uint64_t);
    uint32_t rest  = this->video_cache.buffer_size % sizeof(uint64_t);

    /* Fast clear, use big integer movements */
    unsigned int i;
    for (i = 0; i < jumps; i++)
        *((uint64_t *)this->video_cache.base + i) = static_cast<uint8_t>(screen::color_e::BLACK);

    /* for sizes < 64 bytes */
    for (unsigned int j = 0; j < rest; j++)
        *((uint8_t *)((uint64_t *)this->video_cache.base + i) + j) =
          static_cast<uint8_t>(screen::color_e::BLACK);

    this->x_offset = 0;
    this->y_offset = 0;

    this->video_cache.actual = this->video_cache.base;

    this->update_video();
}

void
fast_renderer_i::scroll()
{
    uint32_t mrg = this->video_cache.ppscl * (this->glyph_y() * 4);
    uint32_t dif = this->video_cache.buffer_size - mrg;
    uint8_t *src = (uint8_t *)this->video_cache.base + mrg;
    uint8_t *dst = (uint8_t *)this->video_cache.base;

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
        uint8_t *clr = ((uint8_t *)this->video_cache.base + this->video_cache.buffer_size) - mrg;

        uint32_t jumps = mrg / sizeof(uint64_t);
        uint32_t rest  = mrg % sizeof(uint64_t);

        unsigned int i;
        for (i = 0; i < jumps; i++)
            *((uint64_t *)clr + i) = 0;

        for (unsigned int j = 0; j < rest; j++)
            *((uint8_t *)((uint64_t *)clr + i) + j) = 0;
    }

    /*
    uint32_t size_clear = this->glyph_y() * this->video_cache.ppscl;

    uint32_t steps     = size_clear / sizeof(uint64_t);
    uint32_t remainder = size_clear % sizeof(uint64_t);

    uint64_t *cache_64 = (uint64_t *)this->video_cache.actual;

    for (uint64_t i = 0; i < steps; i++)
        *cache_64++ = 0;

    uint8_t *cache_8 = (uint8_t *)cache_64;

    for (uint64_t i = 0; i < remainder; i++)
        *cache_8++ = 0;

    this->video_cache.actual += this->video_cache.ppscl * this->glyph_y();
    if (this->video_cache.actual >= this->video_cache.limit)
        this->video_cache.actual = this->video_cache.base;
    */

    if (this->y_offset >= this->glyph_y())
        this->y_offset -= this->glyph_y();

    this->update_video();
}

unsigned int
fast_renderer_i::get_x()
{
    return this->x_offset;
}

unsigned int
fast_renderer_i::get_y()
{
    return this->y_offset;
}

void
fast_renderer_i::set_x(unsigned int value)
{
    this->x_offset = value;
}

void
fast_renderer_i::set_y(unsigned int value)
{
    this->y_offset = value;
}

void
fast_renderer_i::fmt(const char *fmtstr, ...)
{
    va_list args;
    va_start(args, fmtstr);
    char buffer[256];
    uint64_t i       = 0;
    uint64_t literal = 0;
    bool specialchar = false;
    while (fmtstr[i] != '\0') {
        if (fmtstr[i] == '%') {
            specialchar = true;
            this->print(&fmtstr[literal], i - literal);
            literal = i + 2;
        } else if (specialchar) {
            specialchar = false;
            switch (fmtstr[i]) {
                case 'i': {
                    str(va_arg(args, int), buffer);
                    this->print(buffer);
                    break;
                }
                case 's': {
                    this->print(va_arg(args, const char *));
                    break;
                }
                case 'p': {
                    hstr(va_arg(args, uint64_t), buffer);
                    this->print("0x");
                    this->print(buffer);
                    break;
                }
                case 'd': {
                    str(va_arg(args, double), buffer);
                    this->print(buffer);
                    break;
                }
                case 'c': {
                    this->printchar((char)va_arg(args, int));
                    break;
                }
            }
        }

        i++;
    }
    this->print(&fmtstr[literal]);
    this->newline();
}

void
fast_renderer_i::update_video()
{
    uint64_t steps     = this->video_memory.buffer_size / sizeof(uint64_t);
    uint64_t remainder = this->video_memory.buffer_size % sizeof(uint64_t);

    uint64_t *cache_64 = (uint64_t *)this->video_cache.actual;
    uint64_t *video_64 = (uint64_t *)this->video_memory.base;

    for (uint64_t i = 0; i < steps; i++)
        *video_64++ = *cache_64++;

    uint8_t *cache_8 = (uint8_t *)cache_64;
    uint8_t *video_8 = (uint8_t *)video_64;

    for (uint64_t i = 0; i < remainder; i++)
        *video_8++ = *cache_8++;
}

} // namespace screen
