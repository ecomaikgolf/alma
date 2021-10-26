/**
 * Page Table Manager
 *
 * K-ary tree struct to map x86-64 virtual addresses to physical addresses. Uses the Page Frame
 * Allocator.
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "address.h"
#include "libc/string.h"
#include "pagetables.h"
#include "uefi/memory.h"

namespace paging {

namespace translator {

/**
 * Trick to have the PGDT page aligned
 *
 * The CPU expects it to be page aligned, i've spent hours debugging why the PTM doesn't works and
 * it was because PGDT wasn't page aligned
 */
struct PGDT_wrapper
{
    page_global_dir_entry_t PGDT[512];
} __attribute__((aligned(uefi::page_size)));

/**
 * Page table Manager
 */
class PTM
{
  public:
    PTM();
    void map(uint64_t, uint64_t);
    static void parse(uint64_t, address_t *);
    static const uint16_t page_size = 512;

    /**
     * Returns the PGDT (PML4 table)
     */
    page_global_dir_entry_t *get_PGDT()
    {
        return this->PGD_table.PGDT;
    }

  private:
    PGDT_wrapper PGD_table;
};

/**
 * @defgroup tablestruct Page Table structures
 *
 * Structures for the Intel x86-64 page table
 *
 * @warning I've used different names for the structs than the ones appearing on the Intel x86
 * manual, i've placed "original" names before each struct
 *
 * @info https://www.iaik.tugraz.at/teaching/materials/os/tutorials/paging-on-intel-x86-64/
 *
 * @{
 */

/**
 * PML4 Entry
 */
struct page_global_dir_entry_t
{
    uint64_t present : 1;
    uint64_t writeable : 1;
    uint64_t user_access : 1;
    uint64_t write_through : 1;
    uint64_t cache_disabled : 1;
    uint64_t accessed : 1;
    uint64_t ignored_3 : 1;
    uint64_t size : 1; // must be 0
    uint64_t ignored_2 : 4;
    uint64_t page_ppn : 28;
    uint64_t reserved_1 : 12; // must be 0
    uint64_t ignored_1 : 11;
    uint64_t execution_disabled : 1;
} __attribute__((__packed__));

/**
 * Dir Pointer Entry
 */
struct page_upper_dir_entry_t
{
    uint64_t present : 1;
    uint64_t writeable : 1;
    uint64_t user_access : 1;
    uint64_t write_through : 1;
    uint64_t cache_disabled : 1;
    uint64_t accessed : 1;
    uint64_t ignored_3 : 1;
    uint64_t size : 1; // 0 means page directory mapped
    uint64_t ignored_2 : 4;
    uint64_t page_ppn : 28;
    uint64_t reserved_1 : 12; // must be 0
    uint64_t ignored_1 : 11;
    uint64_t execution_disabled : 1;
} __attribute__((__packed__));

/**
 * Directory Entry
 */
struct page_mid_dir_entry_t
{
    uint64_t present : 1;
    uint64_t writeable : 1;
    uint64_t user_access : 1;
    uint64_t write_through : 1;
    uint64_t cache_disabled : 1;
    uint64_t accessed : 1;
    uint64_t ignored_3 : 1;
    uint64_t size : 1; // 0 means page table mapped
    uint64_t ignored_2 : 4;
    uint64_t page_ppn : 28;
    uint64_t reserved_1 : 12; // must be 0
    uint64_t ignored_1 : 11;
    uint64_t execution_disabled : 1;
} __attribute__((__packed__));

/**
 * Page Table Entry
 */
struct page_table_entry_t
{
    uint64_t present : 1;
    uint64_t writeable : 1;
    uint64_t user_access : 1;
    uint64_t write_through : 1;
    uint64_t cache_disabled : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t size : 1;
    uint64_t global : 1;
    uint64_t ignored_2 : 3;
    uint64_t page_ppn : 28;
    uint64_t reserved_1 : 12; // must be 0
    uint64_t ignored_1 : 11;
    uint64_t execution_disabled : 1;
} __attribute__((__packed__));
/** @} */ // end of tablestruct

} // namespace translator

} // namespace paging
