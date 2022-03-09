/**
 * Trivial allocator
 */

#include "heap/trivial_allocator.h"
#include "kernel.h"

namespace heap {

void *
trivial_allocator::malloc(uint64_t size)
{
    uint32_t pages = size / kernel::page_size + 1;

    // auto aux = kernel::allocator.request_page();
    //  kernel::allocator.lock_pages(aux, (uint64_t)pages);
    return nullptr;
}

void
trivial_allocator::free(void *addr)
{
    // Who needs it?
    return;
}
} // namespace heap
