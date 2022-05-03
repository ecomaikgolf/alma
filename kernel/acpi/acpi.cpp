/**
 * Advanced Configuration & Power Interface (ACPI) source file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "acpi/acpi.h"
#include "kernel.h"
#include "lib/string.h"
#include <stdint.h>

namespace acpi {

/**
 * Print system ACPI tables
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
 * Find a certain table by it's signature
 *
 * @arg signature Table signature
 * @return pointer to the table (or null if not found)
 */
sdt *
rsdp_v2::find_table(const char *signature)
{
    acpi::sdt *xsdt = (acpi::sdt *)this->xsdt;

    int entries = (xsdt->length - sizeof(acpi::sdt)) / sizeof(uint64_t);

    for (int i = 0; i < entries; i++) {
        acpi::sdt *tbl =
          (acpi::sdt *)*(uint64_t *)((uint64_t)xsdt + sizeof(acpi::sdt) + (i * sizeof(uint64_t)));
        if (tbl->check_signature(signature))
            return tbl;
    }

    /* No table found */
    return nullptr;
}

/**
 * Check if signature is valid
 *
 * @arg signature correct signature
 * @return true/false if correct or not
 */
bool
sdt::check_signature(const char *signature)
{
    return (strncmp(this->signature, signature, 4) == 0);
}

} // namespace acpi
