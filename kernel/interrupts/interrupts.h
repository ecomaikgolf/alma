
#pragma once
#include "io/bus.h"
#include "screen/renderer.h"

namespace interrupts {
struct frame;

__attribute__((interrupt)) void reserved(frame *);
__attribute__((interrupt)) void keyboard(frame *);

} // namespace interrupts
