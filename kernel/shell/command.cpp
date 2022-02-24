#include "shell/command.h"
#include "kernel.h"
#include "libc/stdlib.h"
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
        kernel::tty.print(") [");
        uint64_t mmio_addr = (i->header.BAR[1] & 0xfffffff0);
        hstr(mmio_addr, buffer);
        kernel::tty.print(buffer);
        kernel::tty.println("]");
    }

    return 0;
}

} // namespace commands

} // namespace shell
