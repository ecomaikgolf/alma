/**
 * Peripheral Component Interconnect (PCI) module
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

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
    /* More parameters needed */
} __attribute__((packed));

struct header_t0
{
    uint8_t BIST;
    uint32_t BAR[6];
    uint32_t cardbus_ptr;
    uint16_t subsystem_vendor;
    uint16_t subsystem;
    uint32_t exp_rom_addr;
    uint8_t capabilities;
    uint8_t reserved;
    uint16_t reserved2;
    uint32_t reserved3;
    uint8_t int_line;
    uint8_t int_pin;
    uint8_t min_gran;
    uint8_t max_lat;
} __attribute__((packed));

struct header_t1
{
    uint8_t BIST;
    uint32_t BAR[2];
    uint8_t primary_bus;
    uint8_t secondary_bus;
    uint8_t subordinate;
    uint8_t secondary_lat;
    uint8_t io_base;
    uint8_t io_limit;
    uint16_t sec_status;
    uint16_t memory_base;
    uint16_t memory_limit;
    uint16_t prefetch_mem_base;
    uint16_t prefetch_mem_limit;
    uint32_t prefetch_upper_base;
    uint32_t prefetch_upper_limit;
    uint16_t io_upper_base;
    uint16_t io_upper_limit;
    uint8_t capabilities;
    uint8_t reserved1;
    uint16_t reserved2;
    uint32_t expansion_rom_addr;
    uint8_t int_line;
    uint8_t int_pin;
    uint16_t bridge_crtl;
} __attribute__((packed));

struct header_t2
{
    uint8_t BIST;
    uint32_t cardbus_socket;
    uint8_t capabilities;
    uint8_t reserved;
    uint16_t secondary_status;
    uint8_t pci_bus_num;
    uint8_t cardbus_bus_num;
    uint8_t subordinate_bus_num;
    uint8_t cardbus_lat_timer;
    uint32_t memory_base_addr_0;
    uint32_t memory_limit_0;
    uint32_t memory_base_addr_1;
    uint32_t memory_limit_1;
    uint32_t io_base_addr_0;
    uint32_t io_limit_0;
    uint32_t io_base_addr_1;
    uint32_t io_limit_1;
    uint8_t int_line;
    uint8_t int_pin;
    uint16_t bridge_crtl;
    uint16_t subsystem_vendor;
    uint16_t subsystem;
    uint32_t legacy_addr;
} __attribute__((packed));

struct pci_device
{
    device_header *header;
    void *header_ext;
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

struct BAR_mem
{
    bool mem : 1;
    unsigned int type : 2;
    bool prefetchable : 3;
    unsigned int addr : 26;
};

struct BAR_io
{
    bool io : 1;
    bool reserved : 1;
    unsigned int addr : 30;
};

class device
{
    device(device_header *);
    device_header header;
    void *header_ext;

    uint16_t segment;
    uint8_t bus;
    uint8_t dev;
    uint8_t fun;
};

} // namespace pci
