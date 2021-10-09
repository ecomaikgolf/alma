
#pragma once
#include "screen/renderer.h"

namespace interrupts {
struct frame;

__attribute__((interrupt)) void pagefault(frame *);

} // namespace interrupts
