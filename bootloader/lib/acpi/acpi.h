/**
 * ACPI Related functionality
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "log/stdout.h"
#include <stdbool.h>
#include <uefi.h>

/**
 * RSDP for ACPI v1
 */
typedef struct
{
    /** Null terminated string that has to be "RSDP PTR " (see the last space) */
    char signature[8];
    /** byte cast of the sum of all bytes must be = 0*/
    uint8_t checksum;
    /** OEM string */
    char oem[6];
    /** ACPI Version */
    uint8_t version;
    /** RSDT physic address*/
    uint32_t rsdt;
} __attribute__((packed)) rsdp_v1;

/**
 * RSDP for ACPI v2
 */
typedef struct
{
    /** Old header before (new items in v2 are "appended" so it's backward compatible) */
    rsdp_v1 header_v1;
    /** table lenght from 0 to end */
    uint32_t length;
    /** XSDT physic address (if ACPI 2.0, use this insead of  header_v1->rsdt, even in 32bit) */
    uint64_t xsdt;
    /** as rsdp_v1->checksum */
    uint8_t checksum_v2;
    /** reserved */
    uint8_t reserved[3];
} __attribute__((packed)) rsdp_v2;

/**
 * SDT Header
 *
 * SDT (Headers for descriptor tables) are divided in two parts, the header (common for all SDT) and
 * the table, different for each one
 */
typedef struct
{
    /** 4 byte string */
    char signature[4];
    /** total lenght of the table, including this header */
    uint32_t length;
    /** version */
    uint8_t revision;
    /** sum of all table bytes (with header) mod 0x100 must be 0 */
    uint8_t checksum;
    char oem[6];
    char oem_table[8];
    uint32_t oem_version;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed)) sdt;

rsdp_v2 *load_rsdp();
bool compare_guid(efi_guid_t *, efi_guid_t *);
