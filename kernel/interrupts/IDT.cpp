/**
 * Interrupt Descriptor
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "interrupts/IDT.h"
#include "interrupts.h"
#include "io/bus.h"
#include "kernel.h"
#include "paging/PFA.h"
#include "uefi/memory.h"

namespace interrupts {

idt_ptr::idt_ptr()
  : lenght(0x0fff)
{}

void
idt_ptr::set_ptr(uint64_t ptr)
{
    this->ptr = ptr;
}

void
idt_ptr::remap_pic(uint8_t ICW2_PIC1, uint8_t ICW2_PIC2)
{
    /* https://wiki.osdev.org/8259_PIC (Initialisation) */

    /* Store the PIC masks */
    uint8_t mask_PIC1, mask_PIC2;
    mask_PIC1 = io::inb(io::PIC1_DATA);
    io::io_wait();
    mask_PIC2 = io::inb(io::PIC2_DATA);
    io::io_wait();

    /* (ICW1) Init master/slave PIC chip */
    io::outb(io::PIC1_COMMAND, io::ICW1_INIT | io::ICW1_ICW4);
    io::io_wait();
    io::outb(io::PIC2_COMMAND, io::ICW1_INIT | io::ICW1_ICW4);
    io::io_wait();

    /* (ICW2) Set vector offset */
    io::outb(io::PIC1_DATA, ICW2_PIC1);
    io::io_wait();
    io::outb(io::PIC2_DATA, ICW2_PIC2);
    io::io_wait();

    /* (ICW3) Tell how master/slave are wired */
    io::outb(io::PIC1_DATA, io::ICW3_PIC1);
    io::io_wait();
    io::outb(io::PIC2_DATA, io::ICW3_PIC2);
    io::io_wait();

    /* (ICW4) Tell PIC we are on 8086 */
    io::outb(io::PIC1_DATA, io::ICW4_8086);
    io::io_wait();
    io::outb(io::PIC2_DATA, io::ICW4_8086);
    io::io_wait();

    /* Restore saved masks */
    io::outb(io::PIC1_DATA, mask_PIC1);
    io::io_wait();
    io::outb(io::PIC2_DATA, mask_PIC2);
    io::io_wait();
}

void
idt_ptr::add_handle(interrupts::vector_e code, void (*handler)(frame *))
{
    interrupts::idt_entry *reserved =
      (interrupts::idt_entry *)(kernel::idtr.ptr +
                                static_cast<int>(code) * sizeof(interrupts::idt_entry));

    reserved->set_offset((uint64_t)handler);
    reserved->vector    = static_cast<uint8_t>(code);
    reserved->type_attr = static_cast<uint8_t>(interrupts::gate_e::interrupt) |
                          static_cast<uint8_t>(interrupts::status_e::enabled);
}

void
idt_entry::set_offset(uint64_t offset)
{
    helper::addr_t *aux = (helper::addr_t *)&offset;

    this->offset_high   = aux->offset_high;
    this->offset_middle = aux->offset_middle;
    this->offset_low    = aux->offset_low;
}

uint64_t
idt_entry::get_offset()
{
    helper::addr_t address = { this->offset_low, this->offset_middle, this->offset_high };

    return *((uint64_t *)&address);
}

} // namespace interrupts
