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
int getpage(int, char **);
int getmac(int, char **);
int vmmap(int, char **);

} // namespace commands

// clang-format off
static const command kernel_commands[] = {
    { "echo"     , &commands::echo },
    { "whoami"   , &commands::whoami },
    { "shell"    , &commands::shell },
    { "clear"    , &commands::clear },
    { "pci"      , &commands::pci},
    { "getpage"  , &commands::getpage},
    { "getmac"   , &commands::getmac},
    { "vmmap"    , &commands::vmmap},
    { nullptr , nullptr }
};
// clang-format on

} // namespace shell
