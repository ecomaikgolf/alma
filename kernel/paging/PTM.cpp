/**
 * Page Table Manager
 *
 * K-ary tree struct to map x86-64 virtual addresses to physical addresses. Uses the Page Frame
 * Allocator.
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "paging/PTM.h"
#include "kernel.h"
#include "paging/PFA.h"

namespace paging {

namespace translator {

/**
 * Page table manager constructor
 *
 * Just sets the PGDT to 0
 */
PTM::PTM()
{
    memset(this->get_PGDT(), 0, uefi::page_size);
}

/**
 * Map a virtual memory address to a physical memory addres using intel's x86-64 paging scheme
 *
 * The table entry scheme forms a K-ary tree with the root at PGD_table (private). Each parent has
 * 512 childrens (as each entry is 8 bytes we have 512 locations (childrens) so we get 4096 bytes
 * of size, a page :) )
 *
 * The functions starts from the root of the tree, moving 1 level at a time, selecting the correct
 * children to move to from the virtual address. Also it has to construct the children subtree if it
 * doesn't exist.
 *
 * During the function you'll see some mysterious 12 bit shifts. That's because we are going to map
 * page aligned addresses, so it doesn't need the last 12 bits (they don't matter). We have to
 * "remove them" when we store it on any entry as the CPU "doesn't want them" but then when WE use
 * it we have to restore the address filling it with zeroes.
 *
 * @info https://www.iaik.tugraz.at/teaching/materials/os/tutorials/paging-on-intel-x86-64/
 * @info http://lenovopress.com/lp1468.pdf
 */
void
PTM::map(uint64_t virt, uint64_t phys)
{
    /* Parse uint64_t bits to a x86-64 virtual address struct */
    address_t *virtaddr = (address_t *)&virt;

    /** Get the page global entry from it's table */
    page_global_dir_entry_t *PGD = &this->get_PGDT()[virtaddr->global];

    /** Get the page upper dir table from the PGD or create & link a new one if needed */
    page_upper_dir_entry_t *PUDT;
    if (!PGD->present) {
        PUDT = (page_upper_dir_entry_t *)kernel::allocator.request_page();
        memset(PUDT, 0, uefi::page_size);
        PGD->page_ppn    = (uint64_t)PUDT >> 12;
        PGD->present     = true;
        PGD->writeable   = true;
        PGD->user_access = true;
    } else {
        PUDT = (page_upper_dir_entry_t *)((uint64_t)PGD->page_ppn << 12);
    }

    /** Get the page upper entry from it's table */
    page_upper_dir_entry_t *PUD = &PUDT[virtaddr->upper];

    /** Get the page mid dir table from the PUD or create & link a new one if needed */
    page_mid_dir_entry_t *PMDT;
    if (!PUD->present) {
        PMDT = (page_mid_dir_entry_t *)kernel::allocator.request_page();
        memset(PMDT, 0, uefi::page_size);
        PUD->page_ppn    = (uint64_t)PMDT >> 12;
        PUD->present     = true;
        PUD->writeable   = true;
        PUD->user_access = true;
    } else {
        PMDT = (page_mid_dir_entry_t *)((uint64_t)PUD->page_ppn << 12);
    }

    /** Get the page mid entry from it's table */
    page_mid_dir_entry_t *PMD = &PMDT[virtaddr->mid];

    /** Get the page table dir table from the PMD or create & link a new one if needed */
    page_table_entry_t *PTDT;
    if (!PMD->present) {
        PTDT = (page_table_entry_t *)kernel::allocator.request_page();
        memset(PTDT, 0, uefi::page_size);
        PMD->page_ppn    = (uint64_t)PTDT >> 12;
        PMD->present     = true;
        PMD->writeable   = true;
        PMD->user_access = true;
    } else {
        PTDT = (page_table_entry_t *)((uint64_t)PMD->page_ppn << 12);
    }

    /** Get the page table entry from it's table */
    page_table_entry_t *PTD = &PTDT[virtaddr->table];

    /** Fill it with the physical address */
    PTD->page_ppn    = (uint64_t)phys >> 12;
    PTD->present     = true;
    PTD->writeable   = true;
    PTD->user_access = true;

    /** Flush TLB Cache Entries  https://www.felixcloutier.com/x86/invlpg */
    asm("invlpg %0" : : "m"(virt));
}

} // namespace translator
} // namespace paging
