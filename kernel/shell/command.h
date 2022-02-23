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

} // namespace commands

static const command kernel_commands[] = { { "echo", &commands::echo },
                                           { "whoami", &commands::whoami },
                                           { nullptr, nullptr } };

} // namespace shell
