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
};

} // namespace screen
