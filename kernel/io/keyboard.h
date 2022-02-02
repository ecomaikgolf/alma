#pragma once

/**
 * PS2 Keyboard
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include <stdint.h>
namespace io {

const char PS2_SCANCODES[] = { 0x0, 0x0,  '1', '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
                               '=', 0x0,  0x0, 'Q', 'W',  'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
                               '[', ']',  0x0, 0x0, 'A',  'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
                               ';', '\'', '`', 0x0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',',
                               '.', '/',  0x0, 0x0, 0x0,  ' ', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                               0x0, 0x0,  0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                               0x0, 0x0,  0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

/**
 * PS2 Keyboards pushes a hex keycode each time a key is pressed
 * then another when it's released, for example:
 *
 * 0x1E : 'a' pressed
 * 0x9E : 'A' released
 *
 * There is no '/' por upper/lower keys, PS2 only comprehends the
 * keys available in the keyboard, upper/lower keys do not exist
 * on it... right?
 *
 * We have to generate them by playing with pressed/released combinations
 */
class ps2
{
  public:
    const static uint8_t max_keycodes = 6;
    /** Adds a new keycode */
    void add_keycode(uint8_t);
    /** Gets the last char */
    char get_last_char();
    /** Checks if we have a keycode combination -> char */
    bool has_char() const;

  private:
    /** Internal keycode "stack" */
    uint8_t keycodes[max_keycodes];
    uint8_t n_keycodes = 0;
    /** Removes queued keycodes */
    void clear_keycodes();
};

} // namespace io
