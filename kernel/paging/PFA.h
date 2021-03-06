/**
 * Kernel's Page Frame Allocator
 *
 * Based on poncho OSDev series allocator
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "lib/bitset.h"
#include "stivale2.h"
#include "uefi/memory.h"
#include <stddef.h>
#include <stdint.h>

namespace paging {

namespace allocator {

/**
 * Page Frame Allocator class
 *
 * Mantains a bitset (size of the total ammount of pages) that indicates free/used pages. You can
 * lock a page/s or free it, you can also request a free page.
 *
 * @info Requesting a free page can be improved with a free list (now it iterates from the start
 * searching for a free page). For now it's out of scope, anyway can be easily implemented I think
 */
class PFA
{
  public:
    PFA(stivale2_struct_tag_memmap *);
    PFA();
    void operator=(PFA &&);
    void free_page(void *);
    void free_pages(void *, uint64_t);
    void lock_page(void *);
    void lock_pages(void *, uint64_t);
    void *request_page();
    std::bitset page;

    /** Gets kernel's total free memory */
    size_t get_free_mem()
    {
        return this->free_mem;
    }

    /** Gets kernel's total reserved (UEFI, ACPI...) memory (unusable for the OS) */
    size_t get_reserved_mem()
    {
        return this->reserved_mem;
    }

    /** Gets kernel's total used memory (OS's used mem, etc) */
    size_t get_used_mem()
    {
        return this->used_mem;
    }

  private:
    void reserve_page(void *);
    void reserve_pages(void *, uint64_t);
    void release_page(void *);
    void release_pages(void *, uint64_t);
    void zero_bitset();
    stivale2_mmap_entry get_largest_segment(stivale2_struct_tag_memmap *);
    size_t free_mem;
    size_t reserved_mem;
    size_t used_mem;
};

} // namespace allocator
} // namespace paging
