/**
 * Interpreter (parse string to execute commands)
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

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

    char *get_buffer()
    {
        return this->input_buffer;
    }

    int process(char *);
    int launch_command(int, char **);
    static const auto BUFFER_SIZE = 256;

  private:
    /** Buffer for user input (shell input) */
    char input_buffer[BUFFER_SIZE];
    /** Array of commands */
    const command *commands;
};

} // namespace shell
