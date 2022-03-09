#include "paging/BPFA.h"
#include "kernel.h"

namespace paging {

namespace allocator {

BPFA::BPFA(stivale2_struct_tag_memmap *map)
{
    if (map == NULL)
        return;

    auto largest = BPFA::get_largest_segment(map);
    /* Pessimistic aproximation */
    auto total_pages = BPFA::get_total_pages(map);

    this->buffer_base = (BPFA_page *)largest.base;
    this->buffer_next = this->buffer_base;
    this->buffer_limi = this->buffer_base + total_pages;

    // memset(this->buffer_base, 0, total_pages * sizeof(BPFA_page));

    for (uint64_t i = 0; i < map->entries; i++) {

        if (map->memmap[i].type == 1) {
            *buffer_next = BPFA_page(nullptr,
                                     nullptr,
                                     map->memmap[i].base,
                                     (uint16_t)(map->memmap[i].length / kernel::page_size),
                                     true);

            if (buffer_next != this->buffer_base) {
                buffer_next->prev       = (buffer_next - 1);
                (buffer_next - 1)->next = this->buffer_next;
            }

            this->list_last = buffer_next;

            buffer_next++;
        }
    }

    this->list_first = this->buffer_base;

    // this->lock_pages(this->buffer_base, total_pages);
}

BPFA &
BPFA::operator=(BPFA &&rval)
{
    this->buffer_base = rval.buffer_base;
    this->buffer_limi = rval.buffer_limi;
    this->buffer_next = rval.buffer_next;
    this->list_first  = rval.list_first;
    this->list_last   = rval.list_last;

    rval.buffer_base = nullptr;
    rval.buffer_limi = nullptr;
    rval.buffer_next = nullptr;
    rval.list_first  = nullptr;
    rval.list_last   = nullptr;

    return *this;
}

bool
BPFA::lock_page(uint64_t addr)
{
    auto iter = this->list_first;
    do {
        if (addr >= iter->addr && addr < (iter->addr + (iter->pages * kernel::page_size))) {
            uint64_t diff    = addr - iter->addr;
            uint16_t pg_diff = diff / kernel::page_size;

            if (diff == 0) {
                iter->addr += kernel::page_size;
                iter->pages -= 1;
            } else if (pg_diff == iter->pages - 1) {
                iter->pages -= 1;
            } else if (iter->pages == 1) {
                iter->remove_node();
                if (iter->prev == nullptr)
                    this->buffer_base = iter->next;
            } else {
                auto newaddr = this->new_node();
                if (newaddr == nullptr)
                    return false;
                iter->split(addr, newaddr);
            }

            return true;
        }
        iter = iter->next;
    } while (iter != nullptr);

    return false;
}

bool
BPFA::lock_pages(uint64_t addr, uint64_t pages)
{
    auto it       = addr;
    uint64_t done = 0;
    for (; pages > 0; pages--) {
        done++;
        if (!this->lock_page(it)) {
            this->free_pages(it, done);
            return false;
        }
        it += kernel::page_size;
    }
    return true;
}

bool
BPFA::free_page(uint64_t addr)
{
    auto newpage = this->new_node();
    if (newpage == nullptr)
        return false;
    newpage->addr       = addr;
    newpage->pages      = 1;
    newpage->occupied   = 1;
    newpage->prev       = this->list_last;
    newpage->prev->next = newpage;
    this->list_last     = newpage;
    return true;
}

bool
BPFA::free_pages(uint64_t addr, uint64_t pages)
{
    for (; pages > 0; pages--) {
        if (!this->free_page(addr))
            return false;
        addr += kernel::page_size;
    }

    return true;
}

inline bool
BPFA::free_page(void *addr)
{
    return this->free_page((uint64_t)addr);
}

inline bool
BPFA::free_pages(void *addr, uint64_t pages)
{
    return this->free_pages((uint64_t)addr, pages);
}

inline bool
BPFA::lock_page(void *addr)
{
    return this->lock_page((uint64_t)addr);
}

inline bool
BPFA::lock_pages(void *addr, uint64_t pages)
{
    return this->lock_pages((uint64_t)addr, pages);
}

void *
BPFA::request_page()
{

    auto iter = this->list_first;
    if (iter == nullptr)
        return nullptr;
    void *retval = (void *)iter->addr;
    this->lock_page(iter->addr);
    return retval;
}

stivale2_mmap_entry
BPFA::get_largest_segment(stivale2_struct_tag_memmap *map)
{
    uint64_t largest_segment_size = 0;
    uint64_t largest_segment      = 0;
    for (uint64_t i = 0; i < map->entries; i++) {
        if (map->memmap[i].type == 1 && map->memmap[i].length > largest_segment_size)
            largest_segment = i;
    }

    return map->memmap[largest_segment];
}

uint64_t
BPFA::get_total_pages(stivale2_struct_tag_memmap *map)
{
    uint64_t total_pages = 0;
    for (uint64_t i = 0; i < map->entries; i++) {
        if (map->memmap[i].type == 1)
            total_pages += map->memmap[i].length / kernel::page_size;
    }

    return total_pages;
}

BPFA_page *
BPFA::new_node()
{
    auto it = this->buffer_next;
    while (true) {
        if (it == this->buffer_limi)
            it = this->buffer_base;

        if (it->occupied == false) {
            this->buffer_next = it + 1;
            return it;
        }

        if (it == this->buffer_next - 1)
            return nullptr;

        it++;
    }
}

void
BPFA_page::remove_node()
{
    this->occupied = false;

    if (this->next != nullptr)
        this->next->prev = this->prev;

    if (this->prev != nullptr)
        this->prev->next = this->next;
}

void
BPFA_page::split(uint64_t addr, BPFA_page *newaddr)
{
    uint64_t diff    = addr - this->addr;
    uint16_t pg_diff = diff / kernel::page_size;

    newaddr->addr     = addr + kernel::page_size;
    newaddr->pages    = this->pages - pg_diff - 1;
    newaddr->occupied = true;

    this->pages = pg_diff;

    newaddr->prev       = this;
    newaddr->next       = this->next;
    newaddr->next->prev = this;
    this->next          = newaddr;
}

} // namespace allocator

} // namespace paging
