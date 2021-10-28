#pragma once

#include "paging/PFA.h"
#include "paging/PTM.h"
#include "screen/fonts/psf1.h"
#include "segmentation/gdt.h"
#include <stdint.h>

/**
 * Size of the kernel
 *
 * Values set by the linker script
 */
extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

namespace kernel {

static paging::allocator::PFA *allocator;
static paging::translator::PTM *translator;
static screen::psf1_renderer *tty;
static segmentation::gdt_ptr *gdt;
const auto page_size = uefi::page_size;

} // namespace kernel
