/**
 * Interpreter (parse string to execute commands)
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "shell/interpreter.h"
#include "kernel.h"
#include "lib/string.h"
#include <stdint.h>

namespace shell {

/**
 * Process shell char string
 */
int
interpreter::process(char *input)
{
    int argc = 0;
    char *argv[256];

    uint32_t charcount = 0;
    bool literal_flag  = false;
    for (uint64_t i = 0; true; i++) {
        if (input[i] == '"') {
            literal_flag = !literal_flag;
            continue;
        }

        if (!literal_flag && (input[i] == ' ' || input[i] == '\n')) {
            if (charcount <= 0)
                continue;

            char *buffer = (char *)kernel::heap.malloc(sizeof(char) * (charcount + 1));

            int32_t j   = i - 1;
            int32_t k   = charcount;
            buffer[k--] = '\0';
            while (j >= 0 && k >= 0 && input[j] != ' ') {
                if (input[j] == '"')
                    continue;
                buffer[k] = input[j];
                j--;
                k--;
            }

            argv[argc++] = buffer;
            charcount    = 0;
            if (input[i] == ' ')
                continue;
            if (input[i] == '\n')
                break;
        }

        charcount++;
    }

    if (argc <= 0)
        return (argc != 0);

    return this->launch_command(argc, argv);
}

/**
 * Find a command and launch it
 *
 * Just find it in a array of <command string, function> (linear search)
 */
int
interpreter::launch_command(int argc, char **argv)
{
    for (uint32_t i = 0; this->commands[i].name != nullptr; i++) {
        if (strcmp(this->commands[i].name, argv[0]) == 0)
            return this->commands[i].function(argc, argv);
    }
    /* Not found notification */
    return 127;
}

} // namespace shell
