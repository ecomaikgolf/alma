/**
 * Kernel "Better" Page Frame Allocator
 *
 * Linked list of pages
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "stivale2.h"
#include <stdint.h>

namespace paging {

namespace allocator {

/**
 * Better Page Frame Allocator Page Class
 *
 * Represents a page (linked list node)
 */
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

/**
 * Better Page Frame Allocator Class
 *
 * Represents the allocator
 */
class BPFA
{
  public:
    BPFA(stivale2_struct_tag_memmap *);
    BPFA()        = default;
    BPFA &operator=(BPFA &&);
    bool free_page(uint64_t);
    bool free_pages(uint64_t, uint64_t);
    bool lock_page(uint64_t);
    bool lock_pages(uint64_t, uint64_t);
    bool free_page(void *);
    bool free_pages(void *, uint64_t);
    bool lock_page(void *);
    bool lock_pages(void *, uint64_t);
    void *request_page(void *ptr = nullptr);
    void *request_cont_page(uint32_t);

    BPFA_page *get_first()
    {
        return this->list_first;
    };
    BPFA_page *get_last()
    {
        return this->list_last;
    };

  private:
    BPFA_page *buffer_base;
    BPFA_page *buffer_next;
    BPFA_page *buffer_limi;

    BPFA_page *list_first;
    BPFA_page *list_last;
    BPFA_page *new_node();

    stivale2_mmap_entry get_largest_segment(stivale2_struct_tag_memmap *);
    uint64_t get_total_pages(stivale2_struct_tag_memmap *);
};

} // namespace allocator

} // namespace paging
