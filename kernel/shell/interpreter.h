#pragma once

#include "command.h"

namespace shell {

class interpreter
{
  public:
    interpreter()
      : commands(nullptr){};
    interpreter(const command *input)
      : commands(input){};
    interpreter &operator=(interpreter &input)
    {
        this->commands = input.commands;
        return *this;
    };

    void loop();
    int process(char *);
    int launch_command(char *, int, char **);

  private:
    /** Buffer for user input (shell input) */
    char input_buffer[256];
    /** Array of commands */
    const command *commands;
};

} // namespace shell
