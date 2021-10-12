#include "interrupts.h"
#include "screen/renderer.h"

namespace interrupts {
__attribute__((interrupt)) void
pagefault(frame *)
{
    // global->println("Hello from the page fault interrupt routine");
    while (true) {
    }
}
} // namespace interrupts
