#include "acpi/acpi.h"

/**
 * Loads RSDP table from EFI config table
 *
 * Iterates through all the tables to find "RSD PTR " signature
 *
 * @return Pointer to rsdp_v2 struct
 */
rsdp_v2 *
load_rsdp()
{
    info("Loading RSDP table");
    efi_configuration_table_t *config_table = ST->ConfigurationTable;
    rsdp_v2 *rsdp                           = NULL;
    efi_guid_t acpi20_table_guid            = ACPI_20_TABLE_GUID;

    for (uintn_t i = 0; i < ST->NumberOfTableEntries; i++) {
        if (compare_guid(&config_table->VendorGuid, &acpi20_table_guid)) {
            if (strncmp("RSD PTR ", config_table->VendorTable, 8)) {
                rsdp = (rsdp_v2 *)config_table->VendorTable;
                info("RSDP table found in %p", rsdp);
                break;
            }
        }
        config_table++;
    }

    if (rsdp == NULL)
        error("RSDP table not found");

    return rsdp;
}

/**
 * Compares GUID from table
 */
bool
compare_guid(efi_guid_t *g1, efi_guid_t *g2)
{
    return (g1->Data1 == g2->Data1 && g1->Data2 == g2->Data2 && g1->Data3 == g2->Data3 &&
            *(uint64_t *)g1->Data4 == *(uint64_t *)g2->Data4);
}
