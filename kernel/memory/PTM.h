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
#include "memory.h"
#include "pagetables.h"
#include "uefimmap.h"
#include "memory.h"

/**
 * Trick to have the PGDT page aligned
 *
 * The CPU expects it to be page aligned, i've spent hours debugging why the PTM doesn't works and
 * it was because PGDT wasn't page aligned
 */
struct PGDT_wrapper
{
    page_global_dir_entry_t PGDT[512];
} __attribute__((aligned(UEFIMMap::page_size)));


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
