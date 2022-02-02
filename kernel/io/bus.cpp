#include "io/bus.h"

namespace io {

void
outb(uint16_t port, uint8_t value)
{
    /* value -> %0 */
    /* port  -> %1 */
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t
inb(uint16_t port)
{
    /* return value -> aux */
    /* port         -> %1  */
    uint8_t aux;
    asm volatile("inb %1, %0" : "=a"(aux) : "Nd"(port));
    return aux;
}

void
io_wait()
{
    /*
     * Send a empty value to the device to "wait"
     */
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

} // namespace io
