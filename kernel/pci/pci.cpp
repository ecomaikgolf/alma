#include "pci.h"
#include "kernel.h"
#include "libc/stdlib.h"

namespace pci {

void
enum_fun(uint64_t addr, uint64_t fun)
{
    uint64_t offset = fun << 12;

    uint64_t funaddr = addr + offset;

    kernel::translator.map(funaddr, funaddr);

    pci::device_header *device = (pci::device_header *)funaddr;

    /* not valid */
    if (device->id == 0 || device->id == 0xffff)
        return;

    char device_id[16];
    hstr(device->id, device_id);
    char vendor_id[16];
    hstr(device->vendor, vendor_id);
    kernel::tty.print(vendor_id);
    kernel::tty.print(" - ");
    kernel::tty.println(device_id);
}

void
enum_dev(uint64_t addr, uint64_t dev)
{
    uint64_t offset = dev << 15;

    uint64_t devaddr = addr + offset;

    kernel::translator.map(devaddr, devaddr);

    pci::device_header *device = (pci::device_header *)devaddr;

    /* not valid */
    if (device->id == 0 || device->id == 0xffff)
        return;

    for (uint64_t fun_i = 0; fun_i < 8; fun_i++)
        enum_fun(devaddr, fun_i);
}

void
enum_bus(uint64_t addr, uint64_t bus)
{
    uint64_t offset = bus << 20;

    uint64_t busaddr = addr + offset;

    kernel::translator.map(busaddr, busaddr);

    pci::device_header *device = (pci::device_header *)busaddr;

    /* not valid */
    if (device->id == 0 || device->id == 0xffff)
        return;

    for (uint64_t dev_i = 0; dev_i < 32; dev_i++)
        enum_dev(busaddr, dev_i);
}

void
enum_pci(acpi::sdt *mcfg)
{
    if (!mcfg->check_signature(pci::MCFG_SIGN))
        return;

    int entries =
      ((mcfg->length) - (sizeof(acpi::sdt) + sizeof(uint64_t))) / sizeof(pci::device_config);

    for (int i = 0; i < entries; i++) {
        /* Table content is after the header, that's why +sizeof(acpi::sdt) */
        pci::device_config *device =
          (pci::device_config *)((uint64_t)mcfg + (sizeof(acpi::sdt) + sizeof(uint64_t)) +
                                 sizeof(pci::device_config) * i);

        for (uint64_t bus = device->start_bus; bus < device->end_bus; bus++)
            enum_bus(device->baseaddr, bus);
    }
}

} // namespace pci
