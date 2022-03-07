#include "shell/command.h"
#include "kernel.h"
#include "lib/stdlib.h"
#include "shell/interpreter.h"

namespace shell {

namespace commands {

int
echo(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        kernel::tty.print(argv[i]);
        if (i + 1 < argc)
            kernel::tty.print(" ");
    }

    return 0;
}

int
whoami(int argc, char **argv)
{
    kernel::tty.print("chad");
    return 0;
}

int
shell(int argc, char **argv)
{
    shell::interpreter inter(shell::kernel_commands);

    while (true) {
        kernel::tty.print("$ ");
        kernel::keyboard.scanf(inter.get_buffer(), inter.BUFFER_SIZE);
        auto ret = inter.process(inter.get_buffer());
        if (ret == 127) {
            kernel::tty.setColor(screen::color_e::RED);
            kernel::tty.print("Command not found");
            kernel::tty.setColor(screen::color_e::WHITE);
        }
        kernel::tty.newline();
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
        hstr(i->header.vendor, buffer);
        kernel::tty.print("* ");
        kernel::tty.print(buffer);
        kernel::tty.print(" - ");
        hstr(i->header.id, buffer);
        kernel::tty.print(buffer);
        kernel::tty.print(" (");
        str(i->device, buffer);
        kernel::tty.print(buffer);
        kernel::tty.print(", ");
        str(i->bus, buffer);
        kernel::tty.print(buffer);
        kernel::tty.print(", ");
        str(i->function, buffer);
        kernel::tty.print(buffer);
        kernel::tty.println(")");
    }

    return 0;
}

int
getpage(int argc, char **argv)
{
    char aux[16];
    void *ptr = kernel::allocator.request_page();
    hstr((uint64_t)ptr, aux);
    kernel::tty.print("0x");
    kernel::tty.println(aux);

    return 0;
}

int
getmac(int argc, char **argv)
{

    char mac_addr[32];
    for (pci::pci_device *i = kernel::devices; i != nullptr; i = i->next) {
        if (i->header.id == 0x8139 && i->header.header_type == 0x0) {

            pci::header_t0 *ext_hdr = (pci::header_t0 *)i->header_ext;
            uint64_t mmio_addr      = (ext_hdr->BAR[1] & 0xfffffff0);

            char auxstr[16];
            for (int i = 0; i < 5; i++) {
                uint8_t aux = *((uint8_t *)mmio_addr + i);
                hstr(aux, auxstr);
                uint8_t endstr      = strlen(auxstr);
                mac_addr[i * 3]     = auxstr[endstr - 2];
                mac_addr[i * 3 + 1] = auxstr[endstr - 1];
                if (i != 4)
                    mac_addr[i * 3 + 2] = ':';
            }
            break;
        }
    }
    mac_addr[14] = '\0';
    kernel::tty.println(mac_addr);

    return 0;
}

int
vmmap(int argc, char **argv)
{
    using namespace paging;
    using namespace paging::translator;

    uint64_t asd                 = 0x7000;
    address_t *virtaddr          = (address_t *)&asd;
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

    char aux[256];
    hstr(phys, aux);
    kernel::tty.println(aux);

    return 0;
}

} // namespace commands

} // namespace shell
