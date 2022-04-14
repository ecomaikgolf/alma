/**
 * Interrupt Functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "interrupts.h"
#include "io/keyboard.h"
#include "kernel.h"
#include "lib/stdlib.h"

namespace interrupts {

/**
 * Reserved special interrupt
 */
__attribute__((interrupt)) void
reserved(frame *)
{
    kernel::tty.println("Hola desde las interrupciones!");
}

/**
 * Keyboard handling interrupt
 */
__attribute__((interrupt)) void
keyboard(frame *)
{
    uint8_t status = io::inb(io::port::PS2);
    /* Return correct receive */
    io::outb(io::PIC1_COMMAND, 0x20);
    kernel::keyboard.process_scancode(status);
}

/**
 * Ethernet packet handling interrupt
 */
__attribute__((interrupt)) void
ethernet(frame *)
{}

} // namespace interrupts
