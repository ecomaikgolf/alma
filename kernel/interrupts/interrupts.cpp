#include "interrupts.h"
#include "io/keyboard.h"
#include "kernel.h"
#include "lib/stdlib.h"
#include "screen/renderer.h"

namespace interrupts {

__attribute__((interrupt)) void
reserved(frame *)
{
    kernel::tty.println("Hola desde las interrupciones!");
}

__attribute__((interrupt)) void
keyboard(frame *)
{
    uint8_t status = io::inb(io::port::PS2);
    io::outb(io::PIC1_COMMAND, 0x20); // return correct receive
    kernel::keyboard.process_scancode(status);

    // char aux[256];
    // hstr(status, aux);
    // kernel::tty.println(aux);
}

__attribute__((interrupt)) void
ethernet(frame *)
{
    kernel::tty.println("Ethernet datagram received");
}

} // namespace interrupts
