/**
 * Interrupt Functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "io/bus.h"

namespace interrupts {

/** Mandatory in order to compile */
struct frame;

__attribute__((interrupt)) void reserved(frame *);
__attribute__((interrupt)) void keyboard(frame *);
__attribute__((interrupt)) void ethernet(frame *);

} // namespace interrupts
