/**
 * Startup functions of the kernel
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "bootstrap/startup.h"
#include "bootstrap/stivale_hdrs.h"
#include "io/bus.h"
#include "kernel.h"
#include "lib/stdlib.h"
#include "paging/BPFA.h"
#include "pci/pci.h"

namespace bootstrap {

/** kernel default keyboard buffer size */
const uint16_t KEYBOARD_BUFF_SIZE = kernel::page_size;

void
screen(stivale2_struct *st)
{
    using namespace screen;

    /* Stivale pointers to the framebuffer & modules */
    auto *fb =
      (stivale2_struct_tag_framebuffer *)stivale2_get_tag(st, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    auto *mod = (stivale2_struct_tag_modules *)stivale2_get_tag(st, STIVALE2_STRUCT_TAG_MODULES_ID);

    /* Create a copy of our framebuffer struct from the data provided by stivale (other format) */
    framebuffer frame;
    frame.base        = (unsigned int *)fb->framebuffer_addr;
    frame.ppscl       = (fb->framebuffer_pitch / sizeof(uint32_t));
    frame.width       = fb->framebuffer_width;
    frame.height      = fb->framebuffer_height;
    frame.buffer_size = frame.ppscl * frame.height * sizeof(uint32_t);

    /* Get the font module from stivale */
    fonts::specification::psf1 *font_ptr =
      (fonts::specification::psf1 *)stivale2_get_mod(mod, "font");

    /* Create the font */
    fonts::specification::psf1 font;
    font.header = *(fonts::specification::psf1_header *)font_ptr;
    font.buffer = (uint8_t *)font_ptr + sizeof(fonts::specification::psf1_header);

    /* Create the tty */
    kernel::tty = fonts::psf1<screen::fast_renderer_i>(frame, font, 0, 0, screen::color_e::WHITE);
}

void
allocator(stivale2_struct *st)
{
    /* stivale pointer to the memory map */
    auto *map = (stivale2_struct_tag_memmap *)stivale2_get_tag(st, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    /* Construct the allocator with the UEFI mem map */
    kernel::allocator = paging::allocator::BPFA(map);
}

void
gdt()
{
    using namespace segmentation;

    /* Create an empty GDT */
    kernel::gdt.size   = sizeof(table) - 1;
    kernel::gdt.offset = (uint64_t)&table;
    /* Load it (assembly) */
    load_gdt(&kernel::gdt);
}

void
translator(stivale2_struct *st)
{
    /*
     * As I'm using limine now I can't create a PGDT from scratch (as limine maps some important
     * addreses) so what I have to do is take the limine's one and edit it, instead of creating a
     * empty one.
     */

    /* Obtain the actual PGDT addr */
    uint64_t mapaddr;
    asm volatile("mov %%cr3, %0" : [Var] "=r"(mapaddr));
    paging::translator::PGDT_wrapper *newpgdt = (paging::translator::PGDT_wrapper *)mapaddr;

    /* Provide it to the translator */
    kernel::translator.set_PGDT(newpgdt);
}

void
interrupts()
{
    /* Reserve memory for the interrupt array */
    kernel::idtr.set_ptr((uint64_t)kernel::allocator.request_page());

    /* Load the interrupt handlers */
    kernel::idtr.add_handle(interrupts::vector_e::reserved, interrupts::reserved);

    /*
     * From OSDev:
     * In protected mode, the IRQs 0 to 7 conflict with the CPU exception which are reserved
     * by Intel up until 0x1F. It is thus recommended to change the PIC's offsets (also known
     * as remapping the PIC) so that IRQs use non-reserved vectors. A common choice is to move
     * them to the beginning of the available range (IRQs 0..0xF -> INT 0x20..0x2F).  For that,
     * we need to set the master PIC's offset to 0x20 and the slave's to 0x28.
     */
    kernel::idtr.remap_pic(0x20, 0x28);

    io::outb(io::PIC1_DATA, 0b11111101); // enable IRQ1 from PIC1 (keyboard)
    io::outb(io::PIC2_DATA, 0b11111111); // mask all lines

    /* Enable interrupts */
    asm("sti");
}

void
enable_virtualaddr()
{
    /* Enable virtual addresses */
    asm("mov %0, %%cr3" : : "r"(kernel::translator.get_PGDT()));
}

void
enable_interrupts()
{
    /* Enable interrupts */
    asm("lidt %0" : : "m"(kernel::idtr));
}

void
keyboard()
{
    /* Reserve keyboard buffer memory */
    auto size   = KEYBOARD_BUFF_SIZE;
    auto buffer = kernel::allocator.request_cont_page(size / kernel::page_size + 1);

    /* Bootstrap the keyboard and enable it */
    kernel::keyboard.set_buffer(static_cast<char *>(buffer));
    kernel::keyboard.set_maxsize(size);
    io::PS2::enable_keyboard();
};

void
acpi(stivale2_struct *st)
{
    /* Find and set the rsdp */
    auto *rsdp   = (stivale2_struct_tag_rsdp *)stivale2_get_tag(st, STIVALE2_STRUCT_TAG_RSDP_ID);
    kernel::rsdp = *(acpi::rsdp_v2 *)rsdp->rsdp;
};

void
pci()
{
    /* Find and set the MCFG table */
    acpi::sdt *mcfg_ptr = (acpi::sdt *)kernel::rsdp.find_table("MCFG");
    /* Enumerate all installed PCI devices */
    pci::enum_pci(mcfg_ptr);
}

void
heap(size_t size)
{
    /* Create the heap */
    kernel::heap = heap::simple_allocator(size);
}

void
rtl8139()
{
    /* From the PCI devices linked list, find the RTL8139 and bootstrap it */
    for (pci::pci_device *i = kernel::devices; i != nullptr; i = i->next) {
        if (i->header->id == 0x8139 && i->header->header_type == 0x0) {
            kernel::rtl8139 = net::rtl8139(i);
            kernel::rtl8139.start();
            return;
        }
    }
}

} // namespace bootstrap
