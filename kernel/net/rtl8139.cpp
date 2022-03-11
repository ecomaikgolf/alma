#include "net/rtl8139.h"
#include "kernel.h"

namespace net {

rtl8139::rtl8139(pci::pci_device *device)
{
    this->device            = device;
    pci::header_t0 *ext_hdr = (pci::header_t0 *)device->header_ext;
    this->io_addr           = (ext_hdr->BAR[1] & 0xfffffff0);
    kernel::translator.map(this->io_addr, this->io_addr);
}

rtl8139 *
rtl8139::operator=(rtl8139 &&rhs)
{
    this->device  = rhs.device;
    this->io_addr = rhs.io_addr;
    rhs.device    = nullptr;
    return this;
}

/**
 * HOTFIX
 */
static inline void
outportb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" ::"a"(val), "d"(port));
}

static inline void
outportw(uint16_t port, uint16_t val)
{
    __asm__ volatile("outw %0, %1" ::"a"(val), "d"(port));
}

static inline void
outportl(uint16_t port, uint32_t val)
{
    __asm__ volatile("outl %0, %1" : : "a"(val), "d"(port));
}

static inline void
outportd(uint16_t port, uint32_t val)
{
    __asm__ volatile("outd %0, %1" : : "a"(val), "d"(port));
}

uint32_t
inportl(uint16_t _port)
{
    uint32_t rv;
    asm volatile("inl %%dx, %%eax" : "=a"(rv) : "dN"(_port));
    return rv;
}

uint16_t
pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp   = 0;

    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) |
                         ((uint32_t)0x80000000));

    // Write out the address
    outportl(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint16_t)((inportl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

void
rtl8139::start()
{
    /**
     * BUG:
     *
     * What is preventing this to start is that I can't update this->device->header->command values
     *
     * MMIO not working
     *
     */
    /** Enable PCI Bus Mastering */
    uint32_t address = (uint32_t)((this->device->bus << 16) | (this->device->device << 11) |
                                  (this->device->function << 8) | ((uint32_t)0x80000000));

    // this->device->header->command |= ((1 << 2) | (1 << 0) | (1 << 1));

    /** Turn on the device */
    // this->setconfig<uint8_t>(rtl8139_config::CONFIG1, 0x0);
    outportb(0xCF8, address | rtl8139_config::CONFIG1);
    outportb(0xCFC, 0x0);

    /** Software reset */
    // this->setconfig<uint8_t>(rtl8139_config::CR, 0x10);
    outportb(0xCF8, address | rtl8139_config::CR);
    outportb(0xCFC, 0x10);
    // while ((this->getconfig<uint8_t>(rtl8139_config::CR) & 0x10) != 0) {
    // }
    while ((pciConfigReadWord(
              this->device->bus, this->device->device, this->device->function, rtl8139_config::CR) &
            0x10) != 0) {
    }

    /** Set receive buffer */
    void *buffer = kernel::heap.malloc(0x8000 + 16);
    // this->setconfig<uintptr_t>(rtl8139_config::RECVBUFF, (uintptr_t)buffer);
    outportl(0xCF8, address | rtl8139_config::RECVBUFF);
    outportl(0xCFC, (uint64_t)buffer);

    /** Set IMR + ISR */
    // this->setconfig<uint32_t>(rtl8139_config::IMR, 0x0005);
    outportl(0xCF8, address | rtl8139_config::IMR);
    outportl(0xCFC, 0x005);

    /** Set RCR */
    // this->setconfig<uint32_t>(rtl8139_config::RCR, 0xf | (1 << 7));
    outportl(0xCF8, address | rtl8139_config::RCR);
    outportl(0xCFC, 0xf | (1 << 7));

    /** Set RE & TE */
    // this->setconfig<uint8_t>(rtl8139_config::CR, 0x0c);
    outportb(0xCF8, address | rtl8139_config::CR);
    outportb(0xCFC, 0x0c);

    auto a = pciConfigReadWord(
      this->device->bus, this->device->device, this->device->function, rtl8139_config::IMR);

    kernel::idtr.add_handle(static_cast<interrupts::vector_e>(32 + a), interrupts::ethernet);
}

void
rtl8139::send_packet(uint32_t addr, uint64_t size)
{
    uint32_t address = (uint32_t)((this->device->bus << 16) | (this->device->device << 11) |
                                  (this->device->function << 8) | ((uint32_t)0x80000000));

    outportl(0xCF8, address | this->TSAD_array[tx_cur]);
    outportl(0xCFC, addr);

    outportl(0xCF8, address | this->TSD_array[tx_cur++]);
    outportl(0xCFC, size);

    if (this->tx_cur > 3)
        this->tx_cur = 0;
}

} // namespace net
