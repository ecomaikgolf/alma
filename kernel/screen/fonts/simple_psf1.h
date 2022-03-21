/**
 * Font definitions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "screen/fonts/psf1.h"
#include "screen/simple_renderer_i.h"

namespace screen {

/**
 * Renderer with PSF1 fonts
 */
class simple_psf1 : public simple_renderer_i
{
  public:
    simple_psf1(framebuffer,
                fonts::psf1,
                unsigned int x_offset = 0,
                unsigned int y_offset = 0,
                color_e color         = color_e::WHITE);
    simple_psf1()        = default;
    simple_psf1 &operator=(simple_psf1 &&);

    void draw(const char);

  private:
    ///** renderer glyph x size */
    unsigned int glyph_x()
    {
        return fonts::psf1::glyph_x;
    }
    ///** renderer glyph y size */
    unsigned int glyph_y()
    {
        return fonts::psf1::glyph_y;
    }
    /** PSF1 font to use */
    fonts::psf1 font;
};

} // namespace screen
