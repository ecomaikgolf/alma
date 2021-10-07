/**
 * Interrupt Descriptor
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "IDT.h"
#include "paging/PFA.h"

namespace interrupts {

idt_ptr::idt_ptr()
  : lenght(0x0fff)
  , ptr((uint64_t)allocator.request_page())
{}

idt_ptr::~idt_ptr()
{
    allocator.free_page((void *)this->ptr);
}

idt_entry::idt_entry(uint64_t addr, uint16_t segment, uint16_t bits)
  : segment(segment)
  , bits(bits)
{
    // HACK Possible source of innumerable bugs
    helper::addr_t *aux = (helper::addr_t *)&addr;

    this->offset_high   = aux->offset_high;
    this->offset_middle = aux->offset_middle;
    this->offset_low    = aux->offset_low;
}

uint64_t
idt_entry::get_offset()
{
    // HACK Possible source of innumerable bugs
    helper::addr_t address = { this->offset_low, this->offset_middle, this->offset_high };

    return *((uint64_t *)&address);
}

} // namespace interrupts
