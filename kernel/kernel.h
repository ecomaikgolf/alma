#pragma once

#include "acpi/acpi.h"
#include "heap/allocator_i.h"
#include "heap/simple_allocator.h"
#include "heap/trivial_allocator.h"
#include "interrupts/IDT.h"
#include "io/keyboard.h"
#include "net/rtl8139.h"
#include "paging/BPFA.h"
#include "paging/PTM.h"
#include "pci/pci.h"
#include "screen/fonts/psf1.h"
#include "segmentation/gdt.h"
#include "shell/interpreter.h"
#include <stdint.h>

namespace kernel {

namespace internal {

/* Set by the linker */
extern "C" uint64_t _start_addr;
extern "C" uint64_t _end_addr;

inline stivale2_struct stivalehdr;

} // namespace internal

/* System Constants  */
const auto page_size = uefi::page_size;

/* Variables */
inline paging::allocator::BPFA allocator;
inline paging::translator::PTM translator __attribute__((aligned(uefi::page_size)));
inline screen::psf1_renderer tty;
inline segmentation::gdt_ptr gdt;
inline interrupts::idt_ptr idtr;
inline io::PS2 keyboard;
inline acpi::rsdp_v2 rsdp;
inline heap::simple_allocator heap;
inline pci::pci_device *devices;
inline net::rtl8139 rtl8139;

/* Kernel Constants */
__attribute__((unused)) static void *_start_addr = &internal::_start_addr;
__attribute__((unused)) static void *_end_addr   = &internal::_end_addr;
const uint64_t _size        = (size_t)&internal::_end_addr - (size_t)&internal::_start_addr;
const uint64_t _size_npages = _size / page_size + 1;

} // namespace kernel
