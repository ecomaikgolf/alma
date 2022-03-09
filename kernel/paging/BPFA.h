
#pragma once

#include "stivale2.h"
#include <stdint.h>

namespace paging {

namespace allocator {

struct BPFA_page
{
    BPFA_page(BPFA_page *next, BPFA_page *prev, uint64_t addr, uint64_t pages, bool free)
      : next(next)
      , prev(prev)
      , addr(addr)
      , pages(pages)
      , occupied(free){};

    BPFA_page *next;
    BPFA_page *prev;
    uint64_t addr;
    uint64_t pages;
    bool occupied = false;

    void remove_node();
    void split(uint64_t, BPFA_page *);
};

class BPFA
{
  public:
    BPFA(stivale2_struct_tag_memmap *);
    BPFA();
    void operator=(BPFA &&);
    void free_page(void *);
    void free_pages(void *, uint64_t);
    void lock_page(uint64_t);
    void lock_pages(void *, uint64_t);
    void *request_page();

    /** Gets kernel's total free memory */
    uint64_t get_free_mem()
    {
        return this->free_mem;
    }

    /** Gets kernel's total reserved (UEFI, ACPI...) memory (unusable for the OS) */
    uint64_t get_reserved_mem()
    {
        return this->reserved_mem;
    }

    /** Gets kernel's total used memory (OS's used mem, etc) */
    uint64_t get_used_mem()
    {
        return this->used_mem;
    }

  private:
    BPFA_page *buffer_base;
    BPFA_page *buffer_next;
    BPFA_page *buffer_limi;
    BPFA_page *get_addr();
    void reserve_page(void *);
    void reserve_pages(void *, uint64_t);
    void release_page(void *);
    void release_pages(void *, uint64_t);
    void zero_bitset();
    stivale2_mmap_entry get_largest_segment(stivale2_struct_tag_memmap *);
    uint64_t get_total_pages(stivale2_struct_tag_memmap *);
    uint64_t free_mem;
    uint64_t reserved_mem;
    uint64_t used_mem;
};

} // namespace allocator

} // namespace paging
