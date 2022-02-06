#include "interrupts.h"
#include "io/keyboard.h"
#include "kernel.h"
#include "libc/stdlib.h"
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
    uint8_t status = io::inb(0x60);   // 0x60 is the keyboard controller port
    io::outb(io::PIC1_COMMAND, 0x20); // return correct receive
    //kernel::keyboard.process_scancode(status);

    // char aux[256];
    // hstr(status, aux);
    // kernel::tty.println(aux);
}

} // namespace interrupts
