/**
 * Trivial allocator
 *
 * This is done to implement interface allocator_i.h and use malloc/free through the code. Then you
 * can change the implementation to a proper allocator
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "heap/trivial_allocator.h"
#include "kernel.h"

namespace heap {

/**
 * Trivial malloc function
 *
 * Try to allocate contiguous pages (no virt mem)
 */
void *
trivial_allocator::malloc(uint64_t size)
{
    uint32_t pages = size / kernel::page_size + 1;

    return kernel::allocator.request_cont_page(pages);
}

/**
 * non-working free
 *
 * It's just here to be able to compile code using the trivial allocator
 */
void
trivial_allocator::free(void *addr)
{
    // Who needs it?
    return;
}
} // namespace heap
