#include "interrupts.h"
#include "kernel.h"
#include "screen/renderer.h"

namespace interrupts {
__attribute__((interrupt)) void
reserved(frame *)
{
    kernel::tty.println("Hola desde las interrupciones!");
}
} // namespace interrupts
