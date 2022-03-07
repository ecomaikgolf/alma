#pragma once

namespace shell {

struct command
{
    const char *name;
    int (*function)(int, char **);
};

namespace commands {

int help(int, char **);
int echo(int, char **);
int whoami(int, char **);
int shell(int, char **);
int clear(int, char **);
int pci(int, char **);
int getpage(int, char **);
int getmac(int, char **);
int getphys(int, char **);
int map(int, char **);

} // namespace commands

// clang-format off
static const command kernel_commands[] = {
    { "help"     , &commands::help},
    { "echo"     , &commands::echo },
    { "whoami"   , &commands::whoami },
    { "shell"    , &commands::shell },
    { "clear"    , &commands::clear },
    { "pci"      , &commands::pci},
    { "getpage"  , &commands::getpage},
    { "getmac"   , &commands::getmac},
    { "getphys"  , &commands::getphys},
    { "map"      , &commands::map},
    { nullptr , nullptr }
};
// clang-format on

} // namespace shell
