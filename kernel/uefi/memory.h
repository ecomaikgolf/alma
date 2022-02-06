/**
 * UEFI memory map utilities
 *
 * Memory map definition based on the posix-uefi definition
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace uefi {

/**
 * Size of a page in bytes
 * Up to 32.767 bytes due to uint16_t
 */
const uint16_t page_size = 4096;

namespace memory {

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
 * UEFI memory segment types
 *
 * @info From posix-uefi and transformed to enum
 */
enum class segment_e
{
    EFI_RESERVED_MEMORY_TYPE,
    EFI_LOADER_CODE,
    EFI_LOADER_DATA,
    EFI_BOOT_SERVICES_CODE,
    EFI_BOOT_SERVICES_DATA,
    EFI_RUNTIME_SERVICES_CODE,
    EFI_RUNTIME_SERVICES_DATA,
    EFI_CONVENTIONAL_MEMORY,
    EFI_UNUSABLE_MEMORY,
    EFI_ACPI_RECLAIM_MEMORY,
    EFI_ACPI_MEMORY_NVS,
    EFI_MEMORY_MAPPED_IO,
    EFI_MEMORY_MAPPED_IO_PORT_SPACE,
    EFI_PAL_CODE,
};

/**
 * Names of UEFI memory segment types (for printing purposes)
 *
 * @info char [][27] as i can't print from a const char* array ¯\_(-.-)_/¯ (27 is the size of the
 * longest string from the array)
 */
extern const char desctypes[][27];

/**
 * UEFI memory map definition
 */
struct map
{
    /** Array of efi_memory_descriptor_t */
    efi_memory_descriptor_t *map;
    /** Total size */
    uint64_t map_size;
    /** Key for current memory map */
    uint64_t map_key;
    /**
     * Size of individual efi_memoryu_descriptor_t
     *
     * @warning You can't do sizeof(efi_memory_descriptor_t)
     */
    uint64_t descriptor_size;
    /** Number of descriptors in the map array */
    uint64_t entries;
} __attribute__((packed));

/** Function to retrieve total memory size from a UEFI memory map */
size_t get_memsize(const map *);

} // namespace memory
} // namespace uefi
