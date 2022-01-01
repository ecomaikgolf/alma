#include "bootstrap/startup.h"
#include "kernel.h"

namespace bootstrap {

void
allocator(uefi::memory::map *map)
{
    kernel::allocator = paging::allocator::PFA(map);
}

void
screen(screen::framebuffer *fb, screen::fonts::psf1 *font)
{
    kernel::tty = screen::psf1_renderer(fb, font, 0, 0, screen::color_e::WHITE);
}

} // namespace bootstrap
