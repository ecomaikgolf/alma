#pragma once

namespace shell {

struct command
{
    const char *name;
    int (*function)(int, char **);
};

namespace commands {

int echo(int, char **);
int whoami(int, char **);
int shell(int, char **);
int clear(int, char **);
int pci(int, char **);

} // namespace commands

// clang-format off
static const command kernel_commands[] = {
    { "echo"  , &commands::echo },
    { "whoami", &commands::whoami },
    { "shell" , &commands::shell },
    { "clear" , &commands::clear },
    { "pci"   , &commands::pci},
    { nullptr , nullptr }
};
// clang-format on

} // namespace shell
