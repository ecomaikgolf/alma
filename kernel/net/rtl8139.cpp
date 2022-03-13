#include "net/rtl8139.h"
#include "kernel.h"

namespace net {

rtl8139::rtl8139(pci::pci_device *device)
{
    this->device            = device;
    pci::header_t0 *ext_hdr = (pci::header_t0 *)device->header_ext;
    this->mem_addr          = (ext_hdr->BAR[1] & 0xfffffff0);
    kernel::translator.map(this->mem_addr, this->mem_addr);
}

rtl8139 *
rtl8139::operator=(rtl8139 &&rhs)
{
    this->device   = rhs.device;
    this->mem_addr = rhs.mem_addr;
    rhs.device     = nullptr;
    return this;
}

void
rtl8139::start()
{
    /** Enable PCI Bus Mastering */
    this->device->header->command |= (1 << 2);

    /** Turn on the device */
    this->setconfig<uint8_t>(rtl8139_config::CONFIG1, 0x0);

    /** Software reset */
    this->setconfig<uint8_t>(rtl8139_config::CR, 0x10);
    while ((this->getconfig<uint8_t>(rtl8139_config::CR) & 0x10) != 0) {
    }

    /** Set receive buffer */
    void *buffer = kernel::allocator.request_page();
    kernel::allocator.request_page();
    kernel::allocator.request_page();
    kernel::allocator.request_page();
    kernel::allocator.request_page();
    kernel::allocator.request_page();
    kernel::allocator.request_page();
    kernel::allocator.request_page();
    kernel::allocator.request_page();
    this->setconfig<uintptr_t>(rtl8139_config::RECVBUFF, (uintptr_t)buffer);

    /** Set IMR + ISR */
    // this->setconfig<uint32_t>(rtl8139_config::IMR, 0x0005);
    this->setconfig<uint16_t>(rtl8139_config::IMR, 0b101);
    this->setconfig<uint16_t>(rtl8139_config::ISR, 0);

    /** Set RCR */
    this->setconfig<uint32_t>(rtl8139_config::RCR, 0b10001111);

    /** Set RE & TE */
    this->setconfig<uint8_t>(rtl8139_config::CR, 0x0c);

    this->tx_cur = 0;

    uint32_t int_line = ((pci::header_t0 *)this->device->header_ext)->int_line;
    kernel::idtr.add_handle(static_cast<interrupts::vector_e>(32 + int_line), interrupts::ethernet);
}

void
rtl8139::send_packet(uint32_t addr, uint64_t size)
{
    // max size = 1792
    this->setconfig<uint32_t>(this->TSAD_array[tx_cur], addr);
    this->setconfig<uint32_t>(this->TSD_array[tx_cur], size);

    tx_cur++;
    /* Circular */
    if (this->tx_cur > 3)
        this->tx_cur = 0;
}

} // namespace net
