/**
 * Trivial allocator
 */

#pragma once

#include "heap/allocator_i.h"
#include <stdint.h>

namespace heap {

class trivial_allocator : allocator_i
{
  public:
    trivial_allocator &operator=(const trivial_allocator &) = default;

    void *malloc(uint64_t);
    void free(void *);
};

} // namespace heap
