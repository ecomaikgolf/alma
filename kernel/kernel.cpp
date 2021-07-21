/**
 * Kernel file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "bootargs.h"
#include "cstr.h"
#include "float.h"
#include "framebuffer.h"
#include "math.h"
#include "renderer.h"

/**
 * Kernel starting function
 *
 * extern C to avoid C++ function mangling
 */
extern "C" void
_start(BootArgs *args)
{
    Renderer renderer(args->fb, args->font);

    renderer.println("Hola esto es con bootargs");

    while (1) {
    }
}
