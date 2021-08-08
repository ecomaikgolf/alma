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
#include "memory/PFA.h"
#include "memory/PTM.h"
#include "renderer.h"
#include "gdt.h"
#include <stddef.h>
#include <stdint.h>

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

/**
 * Kernel starting function
 * extern C to avoid C++ function mangling
 */
extern "C" void
_start(BootArgs *args)
{
    Renderer renderer(args->fb, args->font);

	gdt_ptr gdt;
	gdt.size = sizeof(table) - 1;
	gdt.offset = (uint64_t)&table;
	load_gdt(&gdt);

    if (args->map == NULL)
        return;

    allocator = PFA(args->map);

    uint64_t _kernel_size  = (size_t)&_kernel_end - (size_t)&_kernel_start;
    uint64_t _kernel_pages = _kernel_size / UEFIMMap::page_size + 1;

    allocator.lock_pages(&_kernel_start, _kernel_pages);

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

    asm("mov %0, %%cr3" : : "r"(page_table.get_PGDT()));

    /** @todo Can be improved doing it with GOP operations in UEFI */
    memset(args->fb->base, 0, args->fb->buffer_size);

    renderer.println("Print con memoria virtual");

    while (1) {
    }
}
