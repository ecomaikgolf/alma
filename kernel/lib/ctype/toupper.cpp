#include "lib/ctype.h"

char
toupper(char letter)
{
    if ((int)letter >= 97 && (int)letter <= 112)
        letter -= 32;

    return letter;
}
