/**
 * UEFI memory map definition
 *
 * Based on the posix-uefi definition
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <stdint.h>

/** posix-uefi typedefs */
typedef uint64_t efi_physical_address_t;
typedef uint64_t efi_virtual_address_t;

/**
 * UEFI memory descriptor, from posix-uefi
 */
struct efi_memory_descriptor_t
{
    uint32_t Type;
    uint32_t Pad;
    efi_physical_address_t PhysicalStart;
    efi_virtual_address_t VirtualStart;
    uint64_t NumberOfPages;
    uint64_t Attribute;
};

/** 
 * UEFI memory map definition
 */
struct MapInfo
{
	efi_memory_descriptor_t *map;
    uint64_t map_size;
    uint64_t map_key;
    uint64_t descriptor_size;
};
