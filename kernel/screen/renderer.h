/**
 * Class to output to the screen
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "colors.h"
#include "framebuffer.h"
#include <stdint.h>

namespace screen {

/**
 * Interface to output to the screen
 */
class renderer_i
{
  public:
    renderer_i() = default;
    void println(const char *);
    void printcharln(char letter)
    {
        char aux[] = { letter, '\0' };
        this->println(aux);
    }
    void print(const char *);
    void printchar(char letter)
    {
        char aux[] = { letter, '\0' };
        this->print(aux);
    };
    void newline()
    {
        this->println("");
    };
    void clear();
    void scroll();
    virtual void draw(const char) = 0;
    void put(const char);
    void setColor(color_e);
    color_e getColor();
    unsigned int get_x();
    unsigned int get_y();
    void set_x(unsigned int);
    void set_y(unsigned int);

  protected:
    /** Framebuffer to use */
    framebuffer fb;
    /** x PIXEL offset of next glyph */
    unsigned int x_offset;
    /** y PIXEL offset of next glyph */
    unsigned int y_offset;
    /** renderer glyph x size */
    virtual unsigned int glyph_x() = 0;
    /** renderer glyph y size */
    virtual unsigned int glyph_y() = 0;
    /** color of next glyph */
    color_e color;
};

} // namespace screen
