/**
 * Kernel's Page Frame Allocator
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "PFA.h"
#include "uefi/memory_map.h"

/* Uninitalised hotxix */
PFA allocator((UEFIMMap::Map *)0x0);

/**
 * Page Frame Allocator constructor
 *
 * Get the largest segment and reserves it for the PFA class (to ensure it won't run out of memory).
 * This is quite hacky and probably in real industry this is done in other (better) way.
 *
 * Then "create" the bitset (set the buffer and size). Size is calculated from
 * UEFIMMap::get_memsize(map) (total UEFI memory). Then we divide it into page size to match 1 bit
 * to 1 page.
 *
 * Then we zero out the bitset and lock the bitset pages.
 *
 * Finally we go through all the UEFI memory map to reserve unusable pages
 *
 * @param map UEFI memory map
 */
PFA::PFA(const UEFIMMap::Map *map)
{
    if (map == NULL)
        return;

    efi_memory_descriptor_t *largest = PFA::get_largest_segment(map);

    size_t memsize     = UEFIMMap::get_memsize(map);
    this->free_mem     = memsize;
    this->reserved_mem = 0;
    this->used_mem     = 0;

    size_t bitset_size = memsize / UEFIMMap::page_size;
    page.set_size(bitset_size);
    page.set_buffer((uint8_t *)largest->PhysicalStart);

    PFA::zero_bitset();

    PFA::lock_pages(this->page.get_buffer(),
                    (this->page.get_size() / (8 * UEFIMMap::page_size)) + 1);

    for (uint64_t i = 0; i < map->entries; i++) {
        efi_memory_descriptor_t *descriptor =
          (efi_memory_descriptor_t *)((uint64_t)map->map + (i * map->descriptor_size));

        if (descriptor->Type != static_cast<int>(UEFIMMap::Segment::EFI_CONVENTIONAL_MEMORY)) {
            this->reserve_pages((void *)descriptor->PhysicalStart, descriptor->NumberOfPages);
        }
    }
}

/**
 * Get the largest *usable* memory segment from a UEFI map
 *
 * @warning Only counts EFI_CONVENTIONAL_MEMORY
 */
efi_memory_descriptor_t *
PFA::get_largest_segment(const UEFIMMap::Map *map)
{
    size_t largest_segment_size                 = 0;
    efi_memory_descriptor_t *largest_descriptor = NULL;
    for (uint64_t i = 0; i < map->entries; i++) {
        efi_memory_descriptor_t *descriptor =
          (efi_memory_descriptor_t *)((uint64_t)map->map + (i * map->descriptor_size));

        if (descriptor->Type == static_cast<int>(UEFIMMap::Segment::EFI_CONVENTIONAL_MEMORY)) {
            if (descriptor->NumberOfPages * 4096 > largest_segment_size) {
                largest_segment_size = descriptor->NumberOfPages * 4096;
                largest_descriptor   = descriptor;
            }
        }
    }

    return largest_descriptor;
}

/**
 * Zero out the page map bitset
 *
 * @todo Move the function to bitset class and call it from here
 */
void
PFA::zero_bitset()
{
    for (size_t i = 0; i < page.get_size() / 8; i++)
        *(uint64_t *)(page.get_buffer() + i) = 0;
}

/**
 * Mark a page as free
 *
 * Updates free_mem and used_mem
 */
void
PFA::free_page(void *addr)
{
    efi_physical_address_t index = (efi_physical_address_t)addr / UEFIMMap::page_size;

    /* already free page */
    if (!this->page[index])
        return;

    this->page.unset(index);

    this->free_mem += UEFIMMap::page_size;
    this->used_mem -= UEFIMMap::page_size;
}

/**
 * Mark a number of pages as free
 *
 * Updates free_mem and used_mem
 */
void
PFA::free_pages(void *addr, uint64_t count)
{
    for (uint64_t i = 0; i < count; i++)
        this->free_page((void *)((uint64_t)addr + (i * UEFIMMap::page_size)));
}

/**
 * Mark a page as locked
 *
 * Updates free_mem and used_mem
 */
void
PFA::lock_page(void *addr)
{
    uint64_t index = (efi_physical_address_t)addr / UEFIMMap::page_size;

    /* already used page */
    if (this->page[index])
        return;

    this->page.set(index);

    this->free_mem -= UEFIMMap::page_size;
    this->used_mem += UEFIMMap::page_size;
}

/**
 * Mark a number of pages as locked
 *
 * Updates free_mem and used_mem
 */
void
PFA::lock_pages(void *addr, uint64_t count)
{
    for (uint64_t i = 0; i < count; i++)
        this->lock_page((void *)((uint64_t)addr + (i * UEFIMMap::page_size)));
}

/**
 * Mark a page as locked
 *
 * Updates free_mem and reserved_mem
 */
void
PFA::reserve_page(void *addr)
{
    uint64_t index = (efi_physical_address_t)addr / UEFIMMap::page_size;

    /* already free page */
    if (!this->page[index])
        return;

    this->page.set(index);

    this->free_mem -= UEFIMMap::page_size;
    this->reserved_mem += UEFIMMap::page_size;
}

/**
 * Mark a number of pages as locked
 *
 * Updates free_mem and used_mem
 */
void
PFA::reserve_pages(void *addr, uint64_t count)
{
    for (uint64_t i = 0; i < count; i++)
        this->reserve_page((void *)((uint64_t)addr + (i * UEFIMMap::page_size)));
}

/**
 * Mark a page as free
 *
 * Updates free_mem and reserved_mem
 */
void
PFA::release_page(void *addr)
{
    uint64_t index = (efi_physical_address_t)addr / UEFIMMap::page_size;

    /* already used page */
    if (this->page[index])
        return;

    this->page.unset(index);

    this->free_mem += UEFIMMap::page_size;
    this->reserved_mem -= UEFIMMap::page_size;
}

/**
 * Mark a number of pages as free
 *
 * Updates free_mem and used_mem
 */
void
PFA::release_pages(void *addr, uint64_t count)
{
    for (uint64_t i = 0; i < count; i++)
        this->release_page((void *)((uint64_t)addr + (i * UEFIMMap::page_size)));
}

/**
 * Request a free page
 *
 * @todo Improve it with a free list or something (for now out of scope as it works^{TM})
 */
void *
PFA::request_page()
{
    for (uint64_t i = 0; i < this->page.get_size(); i++) {
        if (this->page[i])
            continue;

        /* Hack to pass the index, as lock_page(addr) gets index by addr / page_size */
        this->lock_page((void *)(i * UEFIMMap::page_size));
        return (void *)(i * UEFIMMap::page_size);
    }

    /* No memory */
    return NULL;
}
