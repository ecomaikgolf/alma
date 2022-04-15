/**
 * Auxiliary functions to comunicate with I/O ports
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "io/bus.h"

namespace io {

/**
 * Send a byte to a port
 */
void
outb(uint16_t port, uint8_t value)
{
    /* value -> %0 */
    /* port  -> %1 */
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * Send a byte to a port (enum version)
 */
void
outb(io::port port, uint8_t value)
{
    io::outb(static_cast<uint16_t>(port), value);
}

/**
 * Receive a byte from a port
 */
uint8_t
inb(uint16_t port)
{
    /* return value -> aux */
    /* port         -> %1  */
    uint8_t aux;
    asm volatile("inb %1, %0" : "=a"(aux) : "Nd"(port));
    return aux;
}

/**
 * Receive a byte from a port (enum version)
 */
uint8_t
inb(io::port port)
{
    return io::inb(static_cast<uint16_t>(port));
}

/**
 * Send a empty value to the device to wait until last operation finished (trick)
 */
void
io_wait()
{
    /*
     * Send a empty value to the device to "wait"
     */
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

} // namespace io
