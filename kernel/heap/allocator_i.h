/**
 * Trivial allocator
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <stdint.h>

namespace heap {

class allocator_i
{
  public:
    virtual void *malloc(uint64_t) = 0;
    virtual void free(void *)      = 0;
};

} // namespace heap
