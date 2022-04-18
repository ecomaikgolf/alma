/**
 * RTL8139 Network Chip Driver
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "pci/pci.h"

namespace net {

/**
 * Configuration registers offsets
 *
 *  http://realtek.info/pdf/rtl8139cp.pdf
 */
enum rtl8139_config
{
    MAC0     = 0x0,
    MAC1     = 0x1,
    MAC2     = 0x2,
    MAC3     = 0x3,
    MAC4     = 0x4,
    MAC5     = 0x5,
    CONFIG1  = 0x52,
    COMMAND  = 0x32,
    RECVBUFF = 0x30,
    IMR      = 0x3c,
    ISR      = 0x3e,
    RCR      = 0x44,
    CR       = 0x37,
    TSAD0    = 0x20,
    TSAD1    = 0x24,
    TSAD2    = 0x28,
    TSAD3    = 0x2c,
    TSD0     = 0x10,
    TSD1     = 0x14,
    TSD2     = 0x18,
    TSD3     = 0x1c,
};

/**
 * RTL8139 Network Device Class
 *
 * Represents the physical device that is able to send packets
 */
class rtl8139
{
  public:
    rtl8139(pci::pci_device *device);
    rtl8139()        = default;
    rtl8139 *operator=(rtl8139 &&);
    void start();
    void send_packet(uint32_t, uint64_t);

    // private:
    static const auto PCI_ID     = 0x8139;
    rtl8139_config TSAD_array[4] = { TSAD0, TSAD1, TSAD2, TSAD3 };
    rtl8139_config TSD_array[4]  = { TSD0, TSD1, TSD2, TSD3 };
    uint8_t tx_cur               = 0; // used to cycle TSAD and TSD arrays

    /** PCI device */
    pci::pci_device *device;
    /** MMIO base address */
    uint64_t mem_addr;

    /**
     * Set a configuration register with MMIO (PCIe)
     */
    template<typename T>
    void setconfig(rtl8139_config reg, T value)
    {
        *((T *)((uint8_t *)this->mem_addr + static_cast<int>(reg))) = value;
    }

    /**
     * Get a configuration register with MMIO (PCIe)
     */
    template<typename T>
    T getconfig(rtl8139_config reg)
    {
        return *((T *)((uint8_t *)this->mem_addr + static_cast<int>(reg)));
    }
};

} // namespace net
