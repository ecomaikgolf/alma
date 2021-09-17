/**
 * Kernel file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "boot/parameters.h"
#include "float.h"
#include "libc/stdlib.h"
#include "paging/PFA.h"
#include "paging/PTM.h"
#include "screen/framebuffer.h"
#include "screen/renderer.h"
#include "segmentation/gdt.h"
#include <stddef.h>
#include <stdint.h>

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

/**
 * Kernel starting function
 * extern C to avoid C++ function mangling
 */
extern "C" [[noreturn]] void
_start(BootArgs *args)
{
	/* Main screen renderer */
    Renderer renderer(args->fb, args->font);

	/* Global descriptor table */
    gdt_ptr gdt;
    gdt.size   = sizeof(table) - 1;
    gdt.offset = (uint64_t)&table;
    load_gdt(&gdt);

	/* Memory allocator */
    allocator = PFA(args->map);

	/* Get kernel size */
    uint64_t _kernel_size  = (size_t)&_kernel_end - (size_t)&_kernel_start;
    uint64_t _kernel_pages = _kernel_size / UEFIMMap::page_size + 1;

	/* Lock kernel's memory */
    allocator.lock_pages(&_kernel_start, _kernel_pages);

	/* Initialise the page table manager */
    PTM page_table;

	/* Map virtual memory to physical memory (same address for the kernel) */
    for (uint64_t i = 0; i < UEFIMMap::get_memsize(args->map); i += UEFIMMap::page_size) {
        page_table.map(i, i);
    }

	/* Lock screen's memory */
    uint64_t fbbase = (uint64_t)args->fb->base;
    uint64_t fbsize = (uint64_t)args->fb->buffer_size + UEFIMMap::page_size;
    allocator.lock_pages((void *)fbbase, fbsize / UEFIMMap::page_size + 1);

	/* Map screen's memory */
	for (uint64_t i = fbbase; i < fbbase + fbsize; i += UEFIMMap::page_size) {
		page_table.map(i, i);
	}

	/* Enable virtual addresses */
    asm("mov %0, %%cr3" : : "r"(page_table.get_PGDT()));

	/* Clean the screen */
	memset(args->fb->base, 0, args->fb->buffer_size);

	renderer.println("Hello from the kernel.");

	/* Shoudln't return */
    while (1) {
    }
}
