#include "bootstrap/startup.h"
#include "kernel.h"
#include "libc/stdlib.h"

namespace bootstrap {

void
screen(screen::framebuffer *fb, screen::fonts::psf1 *font)
{
    /* Lock the tty itself  */
    kernel::allocator.lock_pages(&kernel::tty, sizeof(kernel::tty) / kernel::page_size + 1);
    /* Create the tty */
    kernel::tty = screen::psf1_renderer(fb, font, 0, 0, screen::color_e::WHITE);
    /* Clean the screen */
    memset(fb->base, 0, fb->buffer_size);

    uint64_t fbbase = (uint64_t)fb->base;
    uint64_t fbsize = (uint64_t)fb->buffer_size + kernel::page_size;
    /* Map screen's memory */
    for (uint64_t i = fbbase; i < fbbase + fbsize; i += kernel::page_size)
        kernel::translator.map(i, i);
}

void
allocator(uefi::memory::map *map)
{
    /* Construct the allocator with the UEFI mem map */
    kernel::allocator = paging::allocator::PFA(map);
    /* Lock kernel memory */
    kernel::allocator.lock_pages(kernel::_start_addr, kernel::_size_npages);
    /* Lock the allocator itself  */
    kernel::allocator.lock_pages(&kernel::allocator,
                                 sizeof(kernel::allocator) / kernel::page_size + 1);
}

void
gdt()
{
    using namespace segmentation;
    /* Lock the GDT */
    kernel::allocator.lock_pages(&kernel::gdt, sizeof(kernel::gdt) / kernel::page_size + 1);
    /* Create an empty GDT */
    kernel::gdt.size   = sizeof(table) - 1;
    kernel::gdt.offset = (uint64_t)&table;
    /* Load it (assembly) */
    load_gdt(&kernel::gdt);
}

void
translator(uefi::memory::map *map)
{
    /* Locks the PTM */
    kernel::allocator.lock_pages(&kernel::translator,
                                 sizeof(kernel::translator) / kernel::page_size + 1);

    /* Map virtual memory to physical memory (same address for the kernel) */
    size_t usable_mem_size = uefi::memory::get_memsize(map);
    for (uint64_t i = 0; i < usable_mem_size; i += kernel::page_size)
        kernel::translator.map(i, i);
}

void
enable_virtualaddr()
{
    /* Enable virtual addresses */
    asm("mov %0, %%cr3" : : "r"(kernel::translator.get_PGDT()));
}

void
enable_interrupts()
{}

} // namespace bootstrap
