/**
 * Commands of the kernel
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

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
int shell(int, char **);
int clear(int, char **);
int pci(int, char **);
int getpage(int, char **);
int getmac(int, char **);
int getphys(int, char **);
int map(int, char **);
int unmap(int, char **);
int set(int, char **);
int get(int, char **);
int printmem(int, char **);
int uefimmap(int, char **);
int printpfa(int, char **);
int checknet(int, char **);
int sendpacket(int, char **);
int screen(int, char **);
int acpi(int, char **);

} // namespace commands

// clang-format off
static const command kernel_commands[] = {
    { "help"       , &commands::help},
    { "echo"       , &commands::echo },
    { "shell"      , &commands::shell },
    { "clear"      , &commands::clear },
    { "pci"        , &commands::pci},
    { "getpage"    , &commands::getpage},
    { "getmac"     , &commands::getmac},
    { "getphys"    , &commands::getphys},
    { "map"        , &commands::map},
    { "unmap"      , &commands::unmap},
    { "get"        , &commands::get},
    { "set"        , &commands::set},
    { "printmem"   , &commands::printmem},
    { "uefimmap"   , &commands::uefimmap},
    { "printpfa"   , &commands::printpfa},
    { "checknet"   , &commands::checknet},
    { "sendpacket" , &commands::sendpacket},
    { "screen"     , &commands::screen},
    { "acpi"       , &commands::acpi},
    { nullptr , nullptr }
};
// clang-format on

} // namespace shell
