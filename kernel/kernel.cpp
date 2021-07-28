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
#include "memory/PTM.h"
#include "memory/PFA.h"
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

    if (args->map == NULL)
        return;

    allocator = PFA(args->map);

    uint64_t _kernel_size  = &_kernel_end - &_kernel_start;
    uint64_t _kernel_pages = _kernel_size / UEFIMMap::page_size + 1;

    allocator.lock_pages(&_kernel_start, _kernel_pages);

    char buffer[120];
    // renderer.println("PAGES: ");
    for (int i = 0; i < 10; i++) {
        hstr((uint64_t)allocator.request_page(), buffer);
        // renderer.println(buffer);
    }

    PTM page_table;

    for (uint64_t i = 0; i < UEFIMMap::get_memsize(args->map); i += UEFIMMap::page_size) {
        page_table.map(i, i);
    }

    uint64_t fbbase = (uint64_t)args->fb->base;
    uint64_t fbsize = (uint64_t)args->fb->buffer_size + UEFIMMap::page_size;

    allocator.lock_pages((void *)fbbase, fbsize / UEFIMMap::page_size + 1);

    for (uint64_t i = fbbase; i < fbbase + fbsize; i += UEFIMMap::page_size) {
        page_table.map(i, i);
    }

    page_table.map(UINT64_MAX, fbbase);

	asm("mov %0, %%cr3" : : "r"(page_table.get_PGDT()));

	uint16_t *a = (uint16_t *)UINT64_MAX;
	*a = 0x1231;

    renderer.println("Print con memoria virtual");

    while (1) {
    }
}
