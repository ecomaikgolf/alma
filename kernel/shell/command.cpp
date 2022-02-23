#include "shell/command.h"
#include "kernel.h"

namespace shell {

namespace commands {

int
echo(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        kernel::tty.print(argv[i]);
        if (i + 1 < argc)
            kernel::tty.print(" ");
    }

    return 0;
}

int
whoami(int argc, char **argv)
{
    kernel::tty.print("chad");
    return 0;
}

} // namespace commands

} // namespace shell
