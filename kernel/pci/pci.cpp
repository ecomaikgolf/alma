/**
 * Peripheral Component Interconnect (PCI) module
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "pci.h"
#include "kernel.h"
#include "lib/stdlib.h"

namespace pci {

/** Auxiliary variables to get the PCI device device/function/bus */
static uint64_t startaddr;
static uint64_t _bus;
static uint64_t startbus;
static uint64_t _device;
static uint64_t _function;

/**
 * Enumerate functions (PCI) and construct the kernel PCI linked list
 */
void
enum_fun(uint64_t addr, uint64_t fun)
{
    _function       = fun;
    uint64_t offset = fun << 12;

    uint64_t funaddr = addr + offset;

    pci::device_header *device = (pci::device_header *)funaddr;

    /* not valid */
    if (device->id == 0 || device->id == 0xffff)
        return;

    static pci_device *prev = nullptr;

    /** Create the linked list of PCI device in the kernel */

    pci::pci_device *dev = (pci::pci_device *)kernel::heap.malloc(sizeof(pci::pci_device));

    dev->header     = device;
    dev->device     = _device;
    dev->function   = _function;
    dev->bus        = _bus;
    dev->prev       = prev;
    dev->header_ext = (void *)((uint8_t *)device + sizeof(pci::device_header));

    /*First device in chain or not */
    if (prev == nullptr)
        kernel::devices = dev;
    else
        prev->next = dev;

    prev = dev;
}

/**
 * Enumerate devices (PCI)
 */
void
enum_dev(uint64_t addr, uint64_t dev)
{
    _device         = dev;
    uint64_t offset = dev << 15;

    uint64_t devaddr = addr + offset;

    pci::device_header *device = (pci::device_header *)devaddr;

    /* not valid */
    if (device->id == 0 || device->id == 0xffff)
        return;

    for (uint64_t fun_i = 0; fun_i < 8; fun_i++) {
        enum_fun(devaddr, fun_i);
    }
}

/**
 * Enumerate bus (PCI)
 */
void
enum_bus(uint64_t addr, uint64_t bus)
{
    _bus            = bus;
    uint64_t offset = bus << 20;

    uint64_t busaddr = addr + offset;

    pci::device_header *device = (pci::device_header *)busaddr;

    /* not valid */
    if (device->id == 0 || device->id == 0xffff)
        return;

    for (uint64_t dev_i = 0; dev_i < 32; dev_i++)
        enum_dev(busaddr, dev_i);
}

/**
 * Enumerate PCI devices
 */
void
enum_pci(acpi::sdt *mcfg)
{
    if (mcfg == nullptr)
        return;

    if (!mcfg->check_signature(pci::MCFG_SIGN))
        return;

    int entries =
      ((mcfg->length) - (sizeof(acpi::sdt) + sizeof(uint64_t))) / sizeof(pci::device_config);

    for (int i = 0; i < entries; i++) {
        /* Table content is after the header, that's why +sizeof(acpi::sdt) */
        pci::device_config *device =
          (pci::device_config *)((uint64_t)mcfg + (sizeof(acpi::sdt) + sizeof(uint64_t)) +
                                 sizeof(pci::device_config) * i);

        startaddr = device->baseaddr;
        startbus  = device->start_bus;

        for (uint64_t bus = device->start_bus; bus < device->end_bus; bus++)
            enum_bus(device->baseaddr, bus);
    }
}

device::device(device_header *hdr)
{
    this->header     = *hdr;
    this->header_ext = (hdr + sizeof(device_header));
}

} // namespace pci
