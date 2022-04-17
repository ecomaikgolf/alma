/**
 * Simple Allocator
 *
 * From "absurponcho" work
 */

#pragma once

#include "heap/allocator_i.h"
#include <stdint.h>

namespace heap {

class simple_allocator : allocator_i
{
  public:
    simple_allocator()
      : heap_address(nullptr)
      , heap_lenght(0){};

    simple_allocator(uint64_t);

    simple_allocator &operator=(const simple_allocator &) = default;

    void *malloc(uint64_t);
    void free(void *);

  private:
    void *heap_address;
    uint64_t heap_lenght;

    static const uint32_t ROUND_NUM = 0x10;

    struct heap_header
    {
        uint64_t length;
        heap_header *next;
        heap_header *last;
        bool is_free;

        heap_header *split(uint64_t lenght);
    };

    void *heap_start;
    void *heap_end;
    heap_header *last_header;

    void expand_heap(uint64_t);
    void combine_forward(heap_header *);
    void combine_backward(heap_header *);
};

} // namespace heap
