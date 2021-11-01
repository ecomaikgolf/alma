#include "bootstrap/startup.h"
#include "kernel.h"

namespace bootstrap {

void
allocator(uefi::memory::map *map)
{
    kernel::allocator = paging::allocator::PFA(map);
}

} // namespace bootstrap
