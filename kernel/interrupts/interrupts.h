#pragma once

#include "io/bus.h"

namespace interrupts {
struct frame;

__attribute__((interrupt)) void reserved(frame *);
__attribute__((interrupt)) void keyboard(frame *);
__attribute__((interrupt)) void ethernet(frame *);

} // namespace interrupts
