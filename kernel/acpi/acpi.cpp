#include "acpi/acpi.h"
#include "kernel.h"
#include "libc/string.h"
#include <stdint.h>

namespace acpi {

/**
 * Print system ACPI tables with kernel::tty
 *
 * @arg rsdp Root System Description Pointer, from UEFI with boot_args
 */
void
rsdp_v2::print_acpi_tables()
{
    acpi::sdt *xsdt = (acpi::sdt *)this->xsdt;

    int entries = (xsdt->length - sizeof(acpi::sdt)) / sizeof(uint64_t);
    for (int i = 0; i < entries; i++) {
        acpi::sdt *tbl =
          (acpi::sdt *)*(uint64_t *)((uint64_t)xsdt + sizeof(acpi::sdt) + (i * sizeof(uint64_t)));
        char str[5] = {
            tbl->signature[0], tbl->signature[1], tbl->signature[2], tbl->signature[3], '\0'
        };
        kernel::tty.println(str);
    }
}

/**
 * Secure ACPI tables by:
 * - Reserving it's memory
 * - Maping the address
 *
 * @arg rsdp Root System Description Pointer, from UEFI with boot_args
 */
void
rsdp_v2::memmap_acpi_tables()
{
    acpi::sdt *xsdt = (acpi::sdt *)this->xsdt;

    /* reserve pages */
    kernel::allocator.lock_pages((void *)xsdt, xsdt->length / kernel::page_size + 1);

    /* map pages */
    for (uint64_t i = 0; i < xsdt->length / kernel::page_size + 1; i += kernel::page_size)
        kernel::translator.map((uint64_t)((uint8_t *)xsdt + i), (uint64_t)((uint8_t *)xsdt + i));
}

sdt *
rsdp_v2::find_table(const char *signature)
{
    acpi::sdt *xsdt = (acpi::sdt *)this->xsdt;

    int entries = (xsdt->length - sizeof(acpi::sdt)) / sizeof(uint64_t);

    for (int i = 0; i < entries; i++) {
        acpi::sdt *tbl =
          (acpi::sdt *)*(uint64_t *)((uint64_t)xsdt + sizeof(acpi::sdt) + (i * sizeof(uint64_t)));
        if (strncmp(tbl->signature, signature, 4) == 0)
            return tbl;
    }

    /* No table found */
    return nullptr;
}

} // namespace acpi
