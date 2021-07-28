/**
 * Memory auxiliary functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "memory.h"
#include <stddef.h>

/**
 * Sets a memory chunk to a certain value
 */
void 
memset(void* addr, uint8_t value, uint64_t size)
{
	/**
	 * @todo Can be optimised usint uint64_t operations if size is big enough
	 */
	for(size_t i = 0 ; i < size ; i++)
		*((uint8_t*)addr + i) = value;
}

