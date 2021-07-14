/**
 * Kernel file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "cstr.h"
#include "framebuffer.h"
#include "renderer.h"

/**
 * Kernel starting function
 *
 * extern C to avoid C++ function mangling
 */
extern "C" void
_start(Framebuffer *fb, PSF1_Font *font)
{
    Renderer renderer(fb, font);
    return;
}
