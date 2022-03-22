/**
 * Class to output to the screen
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "screen/framebuffer.h"
#include "screen/renderer_i.h"
#include <stdarg.h>
#include <stdint.h>

namespace screen {

/**
 * Interface to output to the screen
 */
class fast_renderer_i : public renderer_i
{
  public:
    fast_renderer_i(framebuffer, unsigned int, unsigned int, color_e);
    fast_renderer_i()             = default;
    virtual void draw(const char) = 0;
    void put(const char);
    void print(const char *, int64_t n = -1);
    void println(const char *);
    void fmt(const char *, ...);
    void newline();
    void clear();
    void scroll();
    void setColor(color_e);
    color_e getColor();
    void pushColor(color_e);
    void popColor();
    uint32_t get_x();
    uint32_t get_y();
    void set_x(uint32_t);
    void set_y(uint32_t);
    void pushCoords(uint32_t, uint32_t);
    void popCoords();
    uint32_t get_width();
    uint32_t get_height();
    /** renderer glyph x size */
    virtual unsigned int glyph_x() = 0;
    /** renderer glyph y size */
    virtual unsigned int glyph_y() = 0;

  protected:
    void draw_pixel(uint32_t, uint32_t);
    uint32_t *get_pixel(uint32_t, uint32_t);
    void update_video();
    void update_line();
    /** Video Memory */
    framebuffer video_memory;
    /** Cache (double buffer) */
    cache_framebuffer video_cache;
    /** x PIXEL offset of next glyph */
    unsigned int x_offset;
    unsigned int alt_x_offset; // backup for push/pop
    /** y PIXEL offset of next glyph */
    unsigned int y_offset;
    unsigned int alt_y_offset; // backup for push/pop
    /** color of next glyph */
    color_e color;
    color_e alt_color;
};

} // namespace screen
