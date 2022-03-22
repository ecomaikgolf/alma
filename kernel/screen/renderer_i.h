/**
 * Renderer interface
 */
#pragma once

#include "screen/colors.h"
#include <stdarg.h>
#include <stdint.h>

namespace screen {

/**
 * Interface to output to the screen
 */
class renderer_i
{
  public:
    renderer_i()                                     = default;
    virtual void draw(const char)                    = 0;
    virtual void put(const char)                     = 0;
    virtual void print(const char *, int64_t n = -1) = 0;
    virtual void println(const char *)               = 0;
    virtual void fmt(const char *, ...)              = 0;
    virtual void newline()                           = 0;
    virtual void clear()                             = 0;
    virtual void scroll()                            = 0;
    virtual void setColor(color_e)                   = 0;
    virtual color_e getColor()                       = 0;
    virtual void pushColor(color_e)                  = 0;
    virtual void popColor()                          = 0;
    virtual void pushCoords(uint32_t, uint32_t)      = 0;
    virtual void popCoords()                         = 0;
    virtual uint32_t get_x(uint32_t)                 = 0;
    virtual uint32_t get_y()                         = 0;
    virtual void set_x(uint32_t)                     = 0;
    virtual void set_y(uint32_t)                     = 0;
    virtual uint32_t get_width()                     = 0;
    virtual uint32_t get_height()                    = 0;
};

} // namespace screen
