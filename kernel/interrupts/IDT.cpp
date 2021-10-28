/**
 * Interrupt Descriptor
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "interrupts/IDT.h"
#include "kernel.h"
#include "paging/PFA.h"
#include "screen/renderer.h"
#include "uefi/memory.h"

namespace interrupts {

idt_ptr::idt_ptr()
  : lenght(0x0fff)
  , ptr((uint64_t)kernel::allocator.request_page())
{}

idt_ptr::~idt_ptr()
{
    kernel::allocator.free_page((void *)this->ptr);
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
