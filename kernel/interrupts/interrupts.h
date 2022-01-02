
#pragma once
#include "screen/renderer.h"

namespace interrupts {
struct frame;

__attribute__((interrupt)) void reserved(frame *);

} // namespace interrupts
