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
    kernel::keyboard.add_keycode(status);
    char a       = kernel::keyboard.get_last_char();
    char text[2] = { a, '\0' };
    if (a != 0x0) {
        kernel::tty.println(text);
    }

    // char a[256];
    // hstr(status, a);
    // kernel::tty.println(a);
}

} // namespace interrupts
