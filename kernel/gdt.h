#pragma once

#include "uefimmap.h"
#include <stdint.h>

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

/**
 * @todo Why code/data segments need 0xa0 (to set the "always 0" bit to 1) ?
 * Setting 0x80 so the bit is 0 breaks the kernel
 * I'm interpreting it correctly ? http://www.osdever.net/bkerndev/Docs/gdt.htm
 */

// clang-format off
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

extern "C" void load_gdt(gdt_ptr*);
