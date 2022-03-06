/**
 * Trivial allocator
 */

#pragma once

#include "heap/allocator_i.h"
#include <stdint.h>

namespace heap {

/**
 * Trivial allocator
 *
 * @warning NAIVE IMPLEMENTATION, DOES NOT WORK ALWAYS
 *
 * We can request a big chunk on memory and get it separated with memory reserved in between. It has
 * to be patched with virtual memory mapping
 */
class trivial_allocator : allocator_i
{
  public:
    trivial_allocator &operator=(const trivial_allocator &) = default;

    void *malloc(uint64_t);
    void free(void *);
};

} // namespace heap
