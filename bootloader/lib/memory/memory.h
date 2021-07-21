/**
 * Functions to manage UEFI memory table
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <uefi.h>

typedef struct
{
	/** Array of memory descriptors */
    efi_memory_descriptor_t *map;
    uintn_t map_size;
    uintn_t map_key;
    uintn_t descriptor_size;
} MapInfo;

MapInfo *load_memmap();
void print_memmap(const MapInfo *);
