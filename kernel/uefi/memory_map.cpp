/**
 * UEFI memory map utilities
 *
 * Memory map definition based on the posix-uefi definition
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "memory_map.h"
#include <stddef.h>
#include <stdint.h>

namespace UEFIMMap {

// clang-format off
const char desctypes[][27] = { 
	"EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode" 
};
// clang-format on

/** Storage for get_memsize (you only need to retrieve it once) */
static size_t memsize = 0;

/**
 * Get total memory size (RAM)
 *
 * @warning If you receive a ammount slightly more than specified on qemu it can be due to
 * virtualisation as other users struggled with it too. Example: I have 512 MB in qemu but this
 * function returns arround 536.47 MB
 */
size_t
get_memsize(const UEFIMMap::Map *map)
{
    if (map == nullptr)
        return 0;

    if (UEFIMMap::memsize > 0)
        return UEFIMMap::memsize;

    for (uint64_t i = 0; i < map->entries; i++) {
        efi_memory_descriptor_t *descriptor =
          (efi_memory_descriptor_t *)((uint64_t)map->map + (i * map->descriptor_size));

        UEFIMMap::memsize += descriptor->NumberOfPages * UEFIMMap::page_size;
    }

    return UEFIMMap::memsize;
}

} // namespace UEFIMMap
