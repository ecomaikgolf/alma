/**
 * Kernel file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "PFA.h"
#include "bootargs.h"
#include "cstr.h"
#include "float.h"
#include "framebuffer.h"
#include "math.h"
#include "renderer.h"
#include <stddef.h>
#include <stdint.h>

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

/**
 * Kernel starting function
 *
 * extern C to avoid C++ function mangling
 */
extern "C" void
_start(BootArgs *args)
{
    Renderer renderer(args->fb, args->font);
    renderer.println("Kernel");

    PFA pfa(args->map);

    uint64_t _kernel_size  = &_kernel_end - &_kernel_start;
    uint64_t _kernel_pages = _kernel_size / UEFIMMap::page_size;

    pfa.lock_pages(&_kernel_start, _kernel_pages);

    while (1) {
    }
}
