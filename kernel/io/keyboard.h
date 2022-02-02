#pragma once

/**
 * PS2 Keyboard
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include <stdint.h>
namespace io {

const char PS2_SCANCODES[][2] = {
    { 0x0, 0x0 },  { 0x0, 0x0 }, { '1', '!' }, { '2', '"' }, { '3', 0x0 }, { '4', '$' },
    { '5', '%' },  { '6', '&' }, { '7', '/' }, { '8', '(' }, { '9', ')' }, { '0', '=' },
    { '\'', '?' }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 'q', 'Q' }, { 'w', 'W' },
    { 'e', 'E' },  { 'r', 'R' }, { 't', 'T' }, { 'y', 'Y' }, { 'u', 'U' }, { 'i', 'I' },
    { 'o', 'O' },  { 'p', 'P' }, { '`', '^' }, { '+', '*' }, { 0x0, 0x0 }, { 0x0, 0x0 },
    { 'a', 'A' },  { 's', 'S' }, { 'd', 'D' }, { 'f', 'F' }, { 'g', 'G' }, { 'h', 'H' },
    { 'j', 'J' },  { 'k', 'K' }, { 'l', 'L' }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
    { 0x0, 0x0 },  { 0x0, 0x0 }, { 'z', 'Z' }, { 'x', 'X' }, { 'c', 'C' }, { 'v', 'V' },
    { 'b', 'B' },  { 'n', 'N' }, { 'm', 'M' }, { ',', ';' }, { '.', ':' }, { '-', '_' },
    { 0x0, 0x0 },  { 0x0, 0x0 }, { 0x0, 0x0 }, { ' ', ' ' }, { 0x0, 0x0 }, { 0x0, 0x0 },
    { 0x0, 0x0 },  { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
    { 0x0, 0x0 },  { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
    { 0x0, 0x0 },  { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
    { 0x0, 0x0 },  { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 }, { 0x0, 0x0 },
    { 0x0, 0x0 },  { 0x0, 0x0 }, { '<', '>' },
};

const unsigned int PS2_SCANCODE_SIZE = sizeof(PS2_SCANCODES) / sizeof(char[2]);

enum class PS2_MODKEYS
{
    L_Ctrl,
    L_Mod,
    L_Alt,
    L_Shift,
    R_Altgr,
    R_Mod,
    R_Ctrl,
    R_Shift
};

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
    /** Process a new scancode */
    void process_scancode(uint8_t);
    /** Delete n chars from buffer */
    void delete_char(uint16_t n = 1);
    /** Add a new char */
    void add_char(char);
    /** True if we need to update keyboard values */
    bool update();
    const char *get_text() const;

  private:
    const static uint16_t BUFFER_SIZE = 256;
    volatile uint16_t buffer_count    = 0;
    char buffer[BUFFER_SIZE];
    bool has_new_key = false;

    struct
    {
        bool mayus = false;
        bool shift = false;
        bool ctrl  = false;
        bool alt   = false;
    } state;
};

} // namespace io
