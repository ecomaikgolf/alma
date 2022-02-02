/**
 * PS2 Keyboard
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "io/keyboard.h"

namespace io {

void
ps2::add_keycode(uint8_t keycode)
{
    if (this->n_keycodes >= max_keycodes - 1) {
        /* Temporary to avoid deadlocks, should be an error */
        this->clear_keycodes();
        return;
    }

    this->keycodes[this->n_keycodes] = keycode;
    this->n_keycodes++;
}

char
ps2::get_last_char()
{
    if (!has_char())
        return (char)0x0;

    /* No remainder check in has_char */
    uint8_t n_keys = this->n_keycodes / 2;

    for (uint8_t i = 0; i < n_keys; i++) {
        uint8_t key1 = this->keycodes[i];
        uint8_t key2 = this->keycodes[i + (n_keys - 1)];

        char code = PS2_SCANCODES[key2];
        return code;

        // switch (key1) {
        //     /* L Shift */
        //     case 0x2a:
        //         break;
        //     /* L Ctrl */
        //     case 0x1d:
        //         break;
        //     /* Alt */
        //     case 0x38:
        //         break;
        //     /* Mod */
        //     case 0xdb:
        //         break;
        //     /* Block Mayus */
        //     case 0x3a:
        //         break;
        // }
    }
    this->clear_keycodes();
}

bool
ps2::has_char() const
{
    return (n_keycodes > 0 && n_keycodes % 2 == 0 &&
            keycodes[0] == (keycodes[n_keycodes - 1] - 0x80));
}

void
ps2::clear_keycodes()
{
    n_keycodes = 0;
}

} // namespace io
