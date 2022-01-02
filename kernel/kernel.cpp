/**
 * Kernel file
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "kernel.h"
#include "bootstrap/startup.h"
#include "float.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "libc/stdlib.h"
#include "paging/PFA.h"
#include "paging/PTM.h"
#include "screen/fonts/psf1.h"
#include "screen/framebuffer.h"
#include "screen/renderer.h"
#include "segmentation/gdt.h"
#include <stddef.h>
#include <stdint.h>

/**
 * Kernel start & end address
 * Set by the linker
 */
// extern uint64_t _kernel_start;
// extern uint64_t _kernel_end;

/**
 * Kernel starting function
 * extern C to avoid C++ function mangling
 */
extern "C" [[noreturn]] void
_start(bootstrap::boot_args *args)
{
    /* Bootstrap the kernel (function order is mandatory) */
    bootstrap::allocator(args->map);
    bootstrap::translator(args->map);
    bootstrap::screen(args->fb, args->font);
    bootstrap::gdt();
    bootstrap::enable_virtualaddr();

    kernel::tty.println("Hello from the kernel");

    interrupts::idt_ptr idtr;
    interrupts::idt_entry *pagefault =
      (interrupts::idt_entry *)(idtr.ptr + static_cast<int>(interrupts::vector_e::page_fault) *
                                             sizeof(interrupts::idt_entry));

    pagefault->set_offset((uint64_t)interrupts::pagefault);
    pagefault->vector    = static_cast<uint8_t>(interrupts::vector_e::page_fault);
    pagefault->type_attr = static_cast<uint8_t>(interrupts::gate_e::interrupt) |
                           static_cast<uint8_t>(interrupts::status_e::enabled);

    asm("lidt %0" : : "m"(idtr));

    asm("int $0x0e");

    /* Shoudln't return */
    while (1) {
    }
}
