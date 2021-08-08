/**
 * Global Descriptor Table managing
 *
 * Creates the global descriptor table, also defines needed structures
 *
 * Segmentation registers (personal note):
 * 	- cs (code segment)
 * 	- ds (data segment)
 * 	- es (extra segment)
 * 	- fs
 * 	- gs
 * 	- ss (stack segment)
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "uefimmap.h"
#include <stdint.h>

/**
 * GDT table entry
 */
struct gdt_entry
{
	/** The lower 16 bits of the limit */
    uint16_t limit_low;
	/** The lower 16 bits of the base */
    uint16_t base_low;
	/** The next 8 bits of the base */
    uint8_t base_middle;
	/**
	 *  7 6 5 4  3  0
	 * ┌─┬───┬──┬────┐
	 * │P│DPL│DT│Type│
	 * └─┴───┴──┴────┘
	 */
    uint8_t access;
	/**
	  *  7 6 5 4 3    0
	  * ┌─┬─┬─┬─┬──────┐
	  * │G│D│0│A│SegLen│
	  * └─┴─┴─┴─┴──────┘
	 */
    uint8_t granularity;
	/** The last 8 bits of the base */
    uint8_t base_high;
} __attribute__((packed));

/**
 * GDT
 */
struct gdt_ptr
{
	/** 
	 * sizeof(gdt) - 1 
	 * @warning Yes, it's -1 always (last valid address)
	 */
    uint16_t size;
	/** &gdt */
    uint64_t offset;
} __attribute__((packed));

/**
 * @todo Why code/data segments need 0xa0 (to set the "always 0" bit to 1) ?
 * Setting 0x80 so the bit is 0 breaks the kernel
 * I'm interpreting it correctly ? http://www.osdever.net/bkerndev/Docs/gdt.htm
 */

// clang-format off
/**
 * Our GDT to load
 *
 * x86-64 architecture requires flat memory model (one segment with a base of 0 and a limit of 
 * 0xFFFFFFFF) 
 *
 * We deal with the GDT for ring sizes and Interrupt Descriptor Tables (segmentation has been
 * replaced (for memory control) with paging)
 */
__attribute__((aligned(UEFIMMap::page_size))) 
const gdt_entry table[] = { 
	/** Kernel null descriptor */
	{ 0, 0, 0, 0, 0, 0 },
	/** 
	 * Kernel code 
	 *
	 * Access:
	 * 	- Present? = 1
	 * 	- Ring = 0
	 * 	- Type? = 1
	 * 	- Which Type = 1010
	 *
	 * Granularity:
	 * 	- Granularity = 1 (4kbyte)
	 * 	- Operand Size = 0 (16bit)
	 * 	- Constant 0
	 * 	- Available for system (0) = 0
	 * 	- Seg Leng = 0
	 */
	{ 0, 0, 0, 0x9a, 0xa0, 0 },
	/** 
	 * Kernel data
	 *
	 * Access:
	 * 	- Present? = 1
	 * 	- Ring = 0
	 * 	- Type? = 1
	 * 	- Which Type = 0010
	 *
	 * Granularity:
	 * 	- Granularity = 1 (4kbyte)
	 * 	- Operand Size = 0 (16bit)
	 * 	- Constant 0
	 * 	- Available for system (0) = 0
	 * 	- Seg Leng = 0
	 */
	{ 0, 0, 0, 0x92, 0xa0, 0 },
	/** User null descriptor */
	{ 0, 0, 0, 0, 0, 0 },
	/** 
	 * User code 
	 *
	 * Access:
	 * 	- Present? = 1
	 * 	- Ring = 0
	 * 	- Type? = 1
	 * 	- Which Type = 1010
	 *
	 * Granularity:
	 * 	- Granularity = 1 (4kbyte)
	 * 	- Operand Size = 0 (16bit)
	 * 	- Constant 0
	 * 	- Available for system (0) = 0
	 * 	- Seg Leng = 0
	 */
	{ 0, 0, 0, 0x9a, 0xa0, 0 },
	/** 
	 * User data
	 *
	 * Access:
	 * 	- Present? = 1
	 * 	- Ring = 0
	 * 	- Type? = 1
	 * 	- Which Type = 0010
	 *
	 * Granularity:
	 * 	- Granularity = 1 (4kbyte)
	 * 	- Operand Size = 0 (16bit)
	 * 	- Constant 0
	 * 	- Available for system (0) = 0
	 * 	- Seg Leng = 0
	 */
	{ 0, 0, 0, 0x92, 0xa0, 0 },
};
// clang-format off

/**
 * Load our GDT
 *
 * @see gdm.asm
 */
extern "C" void load_gdt(gdt_ptr*);
