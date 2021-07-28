/**
 * Functions to manage UEFI memory table
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <uefi.h>

/** num of tries to get correct memory size */
static const uint8_t RETRIES = 5;

typedef struct
{
	/** Array of memory descriptors */
    efi_memory_descriptor_t *map;
    uint64_t map_size;
    uint64_t map_key;
    uint64_t descriptor_size;
    uint64_t entries;
} MapInfo;

MapInfo *load_memmap();
void print_memmap(const MapInfo *);
