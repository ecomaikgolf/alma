/**
 * Class to output to the screen
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "colors.h"
#include "font.h"
#include "framebuffer.h"

/**
 * Simple output to the screen
 */
class Renderer
{
  public:
    Renderer(Framebuffer *,
             PSF1_Font *,
             unsigned int x_offset = 0,
             unsigned int y_offset = 0,
             Color color           = Color::WHITE);

    void println(const char *);
    void print(const char *);
    void draw(const char);
    void put(const char);
    void setColor(Color);

  private:
    /** Framebuffer to use */
    Framebuffer *fb;
    /** PSF1 font to use */
    PSF1_Font *font;
    /** x PIXEL offset of next glyph */
    unsigned int x_offset;
    /** y PIXEL offset of next glyph */
    unsigned int y_offset;
    /** color of next glyph */
    Color color;
};

extern Renderer *screen;
