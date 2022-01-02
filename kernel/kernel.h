#pragma once

#include "interrupts/IDT.h"
#include "paging/PFA.h"
#include "paging/PTM.h"
#include "screen/fonts/psf1.h"
#include "segmentation/gdt.h"
#include <stdint.h>

namespace kernel {

namespace internal {

/* Set by the linker */
extern "C" uint64_t _start_addr;
extern "C" uint64_t _end_addr;

} // namespace internal

/* System Constants  */
const auto page_size = uefi::page_size;

/* Variables */
inline paging::allocator::PFA allocator;
inline paging::translator::PTM translator __attribute__((aligned(uefi::page_size)));
inline screen::psf1_renderer tty;
inline segmentation::gdt_ptr gdt;
inline interrupts::idt_ptr idtr;

/* Kernel Constants */
__attribute__((unused)) static void *_start_addr = &internal::_start_addr;
__attribute__((unused)) static void *_end_addr   = &internal::_end_addr;
const uint64_t _size        = (size_t)&internal::_end_addr - (size_t)&internal::_start_addr;
const uint64_t _size_npages = _size / page_size + 1;

} // namespace kernel
