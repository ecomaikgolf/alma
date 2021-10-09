#include "interrupts.h"

namespace interrupts {
__attribute__((interrupt)) void
pagefault(frame *fr)
{
    screen->println("Hello from the page fault interrupt routine");
    while (true) {
    }
}
} // namespace interrupts
