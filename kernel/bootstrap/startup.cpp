#include "bootstrap/startup.h"
#include "kernel.h"

namespace bootstrap {

void
screen(screen::framebuffer *fb, screen::fonts::psf1 *font)
{
    kernel::tty = screen::psf1_renderer(fb, font, 0, 0, screen::color_e::WHITE);
}

void
allocator(uefi::memory::map *map)
{
    /** Construct the allocator with the UEFI mem map */
    kernel::allocator = paging::allocator::PFA(map);
    kernel::allocator.lock_pages(kernel::_start_addr, kernel::_size_npages);
}

void
gdt()
{
    using namespace segmentation;

    kernel::gdt.size   = sizeof(table) - 1;
    kernel::gdt.offset = (uint64_t)&table;
    load_gdt(&kernel::gdt);
}

} // namespace bootstrap
