/**
 * Commands of the kernel
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "shell/command.h"
#include "bootstrap/stivale_hdrs.h"
#include "kernel.h"
#include "lib/stdlib.h"
#include "shell/interpreter.h"

namespace shell {

namespace commands {

int
help(int argc, char **argv)
{
    for (uint32_t i = 0; kernel_commands[i].name != nullptr; i++) {
        kernel::tty.print(kernel_commands[i].name);
        if (kernel_commands[i + 1].name != nullptr)
            kernel::tty.print(", ");
        else
            kernel::tty.newline();
    }

    return 0;
}

int
echo(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        kernel::tty.print(argv[i]);
        if (i + 1 < argc)
            kernel::tty.print(" ");
    }
    kernel::tty.newline();

    return 0;
}

int
shell(int argc, char **argv)
{
    shell::interpreter inter(shell::kernel_commands);

    while (true) {
        kernel::tty.pushColor(screen::color_e::GREEN);
        kernel::tty.print("$ ");
        kernel::tty.popColor();
        kernel::keyboard.scanf(inter.get_buffer(), inter.BUFFER_SIZE);
        auto ret = inter.process(inter.get_buffer());
        if (ret == 127) {
            kernel::tty.pushColor(screen::color_e::RED);
            kernel::tty.println("Command not found");
            kernel::tty.popColor();
        }
    }

    return 0;
}

int
clear(int argc, char **argv)
{
    kernel::tty.clear();
    return 0;
}

int
pci(int argc, char **argv)
{
    char buffer[256];
    for (pci::pci_device *i = kernel::devices; i != nullptr; i = i->next) {
        hstr(i->header->vendor, buffer);
        kernel::tty.fmt("* %p - %p", i->header->vendor, i->header->id);
    }

    return 0;
}

int
getpage(int argc, char **argv)
{
    if (argc >= 2) {
        uint64_t addr = strol(argv[1], 16);
        auto ret      = kernel::allocator.request_page((void *)addr);
        if (ret == nullptr) {
            kernel::tty.println("Not available");
            return 1;
        }
        kernel::tty.fmt("%p", (uint64_t)ret);
        return 0;
    }

    kernel::tty.fmt("%p", (uint64_t)kernel::allocator.request_page());
    return 0;
}

int
getmac(int argc, char **argv)
{

    char mac_addr[32];
    for (pci::pci_device *i = kernel::devices; i != nullptr; i = i->next) {
        if (i->header->id == 0x8139 && i->header->header_type == 0x0) {

            pci::header_t0 *ext_hdr = (pci::header_t0 *)i->header_ext;
            uint64_t mmio_addr      = (ext_hdr->BAR[1] & 0xfffffff0);

            char auxstr[16];
            for (int i = 0; i < 6; i++) {
                uint8_t aux = *((uint8_t *)mmio_addr + i);
                hstr(aux, auxstr);
                uint8_t endstr      = strlen(auxstr);
                mac_addr[i * 3]     = auxstr[endstr - 2];
                mac_addr[i * 3 + 1] = auxstr[endstr - 1];
                if (i != 5)
                    mac_addr[i * 3 + 2] = ':';
            }
            break;
        }
    }
    mac_addr[17] = '\0';
    kernel::tty.println(mac_addr);

    return 0;
}

int
getphys(int argc, char **argv)
{
    using namespace paging;
    using namespace paging::translator;

    if (argc <= 1) {
        kernel::tty.fmt("Usage: %s virtaddr", argv[0]);
        return 1;
    }

    uint64_t addr                = strol(argv[1], 16);
    address_t *virtaddr          = (address_t *)&addr;
    page_global_dir_entry_t *PGD = &kernel::translator.get_PGDT()[virtaddr->global];

    if (!PGD->present)
        return 1;

    page_upper_dir_entry_t *PUDT = (page_upper_dir_entry_t *)((uint64_t)PGD->page_ppn << 12);
    page_upper_dir_entry_t *PUD  = &PUDT[virtaddr->upper];

    if (!PUD->present)
        return 1;

    page_mid_dir_entry_t *PMDT = (page_mid_dir_entry_t *)((uint64_t)PUD->page_ppn << 12);
    page_mid_dir_entry_t *PMD  = &PMDT[virtaddr->mid];

    if (!PMD->present)
        return 1;

    page_table_entry_t *PTDT = (page_table_entry_t *)((uint64_t)PMD->page_ppn << 12);
    page_table_entry_t *PTD  = &PTDT[virtaddr->table];

    uint64_t phys = (uint64_t)PTD->page_ppn << 12;

    kernel::tty.fmt("%p", phys);

    return 0;
}

int
map(int argc, char **argv)
{
    if (argc <= 2) {
        kernel::tty.fmt("Usage: %s virtaddr physaddr", argv[0]);
        return 1;
    }

    uint64_t virt = strol(argv[1], 16);
    uint64_t phys = strol(argv[2], 16);

    kernel::translator.map(virt, phys);

    kernel::tty.fmt("%s -> %s", argv[1], argv[2]);

    return 0;
}

int
unmap(int argc, char **argv)
{
    if (argc <= 1) {
        kernel::tty.fmt("Usage: %s virtaddr", argv[0]);
        return 1;
    }

    uint64_t virt = strol(argv[1], 16);

    kernel::translator.map(virt, virt);

    kernel::tty.fmt("%s -> %s", argv[1], argv[1]);

    return 0;
}

int
set(int argc, char **argv)
{
    if (argc <= 2) {
        kernel::tty.fmt("Usage: %s addr true/false", argv[0]);
        return 1;
    }

    bool set = (strncmp(argv[2], "true", 4) == 0);

    uint64_t addr = strol(argv[1], 16);
    bool *data    = (bool *)addr;
    *data         = set;

    kernel::tty.print("*(bool *)");
    kernel::tty.print(argv[1]);
    if (*data)
        kernel::tty.println(" = true");
    else
        kernel::tty.println(" = false");

    return 0;
}

int
get(int argc, char **argv)
{
    if (argc <= 1) {
        kernel::tty.fmt("Usage: %s addr", argv[0]);
        return 1;
    }

    uint64_t addr = strol(argv[1], 16);
    bool *data    = (bool *)addr;
    kernel::tty.print("*(bool *)");
    kernel::tty.print(argv[1]);
    if (*data)
        kernel::tty.println(" -> true");
    else
        kernel::tty.println(" -> false");
    return 0;
}

int
printmem(int argc, char **argv)
{
    /** Constants */
    static const uint32_t BYTES_PER_LINE = 16;
    static const uint32_t DEFAULT_LINES  = kernel::page_size / BYTES_PER_LINE;

    if (argc <= 1) {
        kernel::tty.fmt("Usage: %s addr [bytes]", argv[0]);
        return 1;
    }

    uint16_t nlines = DEFAULT_LINES;
    if (argc == 3)
        nlines = strol(argv[2], 10);

    uint64_t addr  = strol(argv[1], 16);
    uint8_t column = 0;
    uint8_t line   = 0;
    for (uint32_t byte = 0; byte < nlines * BYTES_PER_LINE; byte++) {

        if (column == 0) {
            char buff[32];
            hstr((uint64_t)(uint8_t *)addr + (line * BYTES_PER_LINE), buff);
            kernel::tty.print(buff);
            kernel::tty.print(": ");
        }

        char buff[16];
        uint8_t *ptr = ((uint8_t *)addr + byte);
        hstr(*ptr, buff);
        int wrongthing = strlen(buff); // TODO: fix this mess
        kernel::tty.put(buff[wrongthing - 2]);
        kernel::tty.put(buff[wrongthing - 1]);
        kernel::tty.put(' ');

        column++;

        if (column != 0 && column % BYTES_PER_LINE == 0) {
            line++;
            column = 0;
            kernel::tty.newline();
        }
    }

    return 0;
}

int
uefimmap(int argc, char **argv)
{
    auto *map = (stivale2_struct_tag_memmap *)stivale2_get_tag(&kernel::internal::stivalehdr,
                                                               STIVALE2_STRUCT_TAG_MEMMAP_ID);

    for (uint64_t i = 0; i < map->entries; i++) {
        auto entry          = map->memmap[i];
        uint64_t init_addr  = entry.base;
        uint64_t fini_addr  = entry.base + entry.length;
        uint64_t kbsize     = entry.length / 1024;
        const char *memtype = nullptr;
        switch (entry.type) {
            case 1:
                memtype = "Usable                ";
                break;
            case 2:
                memtype = "Reserved              ";
                break;
            case 3:
                memtype = "ACPI Reclaimable      ";
                break;
            case 4:
                memtype = "ACPI NVS              ";
                break;
            case 5:
                memtype = "Bad Memory            ";
                break;
            case 0x1000:
                memtype = "Bootloader Reclaimable";
                break;
            case 0x1001:
                memtype = "Kernel and Modules    ";
                break;
            case 0x1002:
                memtype = "Framebuffer           ";
                break;
            default:
                memtype = "ERROR                 ";
        }
        kernel::tty.fmt("%p - %p %s [%i KB]", init_addr, fini_addr, memtype, kbsize);
    }

    return 0;
}

int
printpfa(int argc, char **argv)
{
    auto it = kernel::allocator.get_first();
    while (it != nullptr) {
        auto limaddr = it->addr + (it->pages * kernel::page_size);
        kernel::tty.fmt("%p - %p [%i pages]", it->addr, limaddr, it->pages);
        it = it->next;
    }

    return 0;
}

int
checknet(int argc, char **argv)
{

    auto hdr  = kernel::rtl8139.device->header;
    auto cmd  = kernel::rtl8139.device->header->command;
    auto addr = kernel::rtl8139.mem_addr;

    auto cf1  = kernel::rtl8139.getconfig<uint8_t>(net::rtl8139_config::CONFIG1);
    auto cr   = kernel::rtl8139.getconfig<uint8_t>(net::rtl8139_config::CR);
    auto buff = kernel::rtl8139.getconfig<uint32_t>(net::rtl8139_config::RECVBUFF);
    auto imr  = kernel::rtl8139.getconfig<uint16_t>(net::rtl8139_config::IMR);
    auto isr  = kernel::rtl8139.getconfig<uint16_t>(net::rtl8139_config::ISR);
    auto rcr  = kernel::rtl8139.getconfig<uint32_t>(net::rtl8139_config::RCR);

    auto m0 = kernel::rtl8139.getconfig<uint8_t>(net::rtl8139_config::MAC0);
    auto m1 = kernel::rtl8139.getconfig<uint8_t>(net::rtl8139_config::MAC1);
    auto m2 = kernel::rtl8139.getconfig<uint8_t>(net::rtl8139_config::MAC2);
    auto m3 = kernel::rtl8139.getconfig<uint8_t>(net::rtl8139_config::MAC3);
    auto m4 = kernel::rtl8139.getconfig<uint8_t>(net::rtl8139_config::MAC4);
    auto m5 = kernel::rtl8139.getconfig<uint8_t>(net::rtl8139_config::MAC5);

    kernel::tty.fmt("BAR[1] & 0xf..f0 :  %p", addr);
    kernel::tty.fmt("pci_dev->command :  %i", cmd);
    kernel::tty.fmt("CF1        (0x52):  %i", cf1);
    kernel::tty.fmt("CR         (0x37):  %i", cr);
    kernel::tty.fmt("BUFF       (0x30):  %p", buff);
    kernel::tty.fmt("IMR        (0x3c):  %i", imr);
    kernel::tty.fmt("ISR        (0x3e):  %i", isr);
    kernel::tty.fmt("RCR:       (0x44):  %i", rcr);

    kernel::tty.fmt("M0:                 %i", m0);
    kernel::tty.fmt("M1:                 %i", m1);
    kernel::tty.fmt("M2:                 %i", m2);
    kernel::tty.fmt("M3:                 %i", m3);
    kernel::tty.fmt("M4:                 %i", m4);
    kernel::tty.fmt("M5:                 %i", m5);

    return 0;
};

int
sendpacket(int argc, char **argv)
{
    struct ethheader
    {
        unsigned char dsta[6];
        unsigned char srca[6];
        uint16_t type;
        unsigned char payload[10];
    } __attribute__((packed));

    auto buffer = (ethheader *)kernel::allocator.request_page();

    kernel::tty.print("> ");

    char text[256];
    kernel::keyboard.scanf(text, 256);

    buffer->dsta[0] = 0xca;
    buffer->dsta[1] = 0xfe;
    buffer->dsta[2] = 0xc0;
    buffer->dsta[3] = 0xff;
    buffer->dsta[4] = 0xee;
    buffer->dsta[5] = 0x00;

    buffer->srca[0] = 0xca;
    buffer->srca[1] = 0xfe;
    buffer->srca[2] = 0xc0;
    buffer->srca[3] = 0xff;
    buffer->srca[4] = 0xee;
    buffer->srca[5] = 0x00;

    buffer->type = 0x0800;

    buffer->payload[0] = text[0];
    buffer->payload[1] = text[1];
    buffer->payload[2] = text[2];
    buffer->payload[3] = text[3];
    buffer->payload[4] = text[4];
    buffer->payload[5] = text[5];
    buffer->payload[6] = text[6];
    buffer->payload[7] = text[7];
    buffer->payload[8] = text[8];
    buffer->payload[9] = text[9];

    kernel::rtl8139.send_packet((uint64_t)buffer, sizeof(ethheader));

    return 0;
}

int
screen(int argc, char **argv)
{
    kernel::tty.fmt("%ix%i", kernel::tty.get_width(), kernel::tty.get_height());
    return 0;
}

int
acpi(int argc, char **argv)
{
    kernel::rsdp.print_acpi_tables();
    return 0;
}

} // namespace commands

} // namespace shell
