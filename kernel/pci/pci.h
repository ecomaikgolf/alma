#pragma once

#include "acpi/acpi.h"

namespace pci {

const char MCFG_SIGN[] = { 'M', 'C', 'F', 'G' };

struct device_config
{
    uint64_t baseaddr;
    uint16_t pci_seg_group;
    uint8_t start_bus;
    uint8_t end_bus;
    uint32_t reserved;
} __attribute__((packed));

struct device_header
{
    /* manufacturer */
    uint16_t vendor;
    uint16_t id;
    uint16_t command;
    uint16_t status;
    uint8_t revision;
    uint8_t program_interface;
    uint8_t dev_subclass;
    uint8_t dev_class;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t BIST;
    uint32_t BAR[6];
    /* More parameters needed */
} __attribute__((packed));

struct pci_device
{
    device_header header;
    uint16_t device;
    uint16_t bus;
    uint16_t function;
    pci_device *prev;
    pci_device *next;
};

void enum_fun(uint64_t addr, uint64_t fun);
void enum_dev(uint64_t addr, uint64_t dev);
void enum_bus(uint64_t addr, uint64_t bus);
void enum_pci(acpi::sdt *);

} // namespace pci
