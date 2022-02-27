/**
 * UEFI memory map utilities
 *
 * Memory map definition based on the posix-uefi definition
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "uefi/memory.h"
#include <stddef.h>
#include <stdint.h>

namespace uefi {

namespace memory {

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

/**
 * Get total memory size (RAM)
 *
 * @warning If you receive a ammount slightly more than specified on qemu it can be due to
 * virtualisation as other users struggled with it too. Example: I have 512 MB in qemu but this
 * function returns arround 536.47 MB
 */
size_t
get_memsize(stivale2_struct_tag_memmap *map)
{
    /** Storage for get_memsize (you only need to retrieve it once) */
    static size_t memsize = 0;

    if (map == nullptr)
        return 0;

    if (memsize > 0)
        return memsize;

    for (uint64_t i = 0; i < map->entries; i++) {
        if (map->memmap[i].type == 1)
            memsize += map->memmap[i].length;
    }

    return memsize;
}

} // namespace memory
} // namespace uefi
