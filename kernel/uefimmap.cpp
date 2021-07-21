#include "uefimmap.h"
#include <stddef.h>
#include <stdint.h>

namespace UEFIMMap {

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
    if (map == NULL)
        return 0;

    if (UEFIMMap::memsize > 0)
        return UEFIMMap::memsize;

    for (int i = 0; i < map->entries; i++) {
        efi_memory_descriptor_t *descriptor =
          (efi_memory_descriptor_t *)((uint64_t)map->map + (i * map->descriptor_size));

        memsize += descriptor->NumberOfPages * UEFIMMap::page_size;
    }

    return UEFIMMap::memsize;
}

} // namespace UEFIMMap
