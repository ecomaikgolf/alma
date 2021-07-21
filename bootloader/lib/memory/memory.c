#include "memory/memory.h"
#include "log/stdout.h"

// clang-format off
char desctypes[][26] = { 
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

MapInfo *
load_memmap()
{
    MapInfo *map = calloc(1, sizeof(MapInfo));

    if (map == NULL) {
        error("can't allocate memory for UEFI memory map info");
        return NULL;
    }

    efi_status_t status =
      BS->GetMemoryMap(&map->map_size, NULL, &map->map_key, &map->descriptor_size, NULL);

    /* status will be EFI_BUFFER_TOO_SMALL as mapsize = 0 and give us the correct size */
    if (status != EFI_BUFFER_TOO_SMALL || map->map_size <= 0 || map->descriptor_size <= 0) {
        /*
         * Check if map->map_size is a reasonable number
         * View page 164 of UEFI Specification 2.8
         */
        error("memory map info retrieval error");
        return NULL;
    }

    map->map = malloc(map->map_size);

    if (map->map == NULL) {
        error("can't allocate memory for UEFI memory map info");
        return NULL;
    }

    status = BS->GetMemoryMap(&map->map_size, map->map, &map->map_key, &map->descriptor_size, NULL);

    if (status > 0) {
        /* Possible errors: view page 164 of UEFI Specification 2.8 */
        error("memory map retrieval error");
        return NULL;
    }

	map->entries = map->map_size / map->descriptor_size;

    if (map->entries == 0)
        warning("memory map without entries");

    return map;
}

void
print_memmap(const MapInfo *map)
{
    uint8_t *start = (uint8_t *)map->map;
    for (int i = 0; i < (map->map_size / map->descriptor_size); i++) {
        efi_memory_descriptor_t *descriptor =
          (efi_memory_descriptor_t *)((uint64_t)start + (i * map->descriptor_size));
        debug("Type %s", desctypes[descriptor->Type]);
        debug("Phy start %p", descriptor->PhysicalStart);
        debug("Number of pages %d", descriptor->NumberOfPages);
        debug("Pad %d\n", descriptor->Pad);
    }
}
