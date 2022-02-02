/**
 * PS2 Keyboard
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "io/keyboard.h"
#include "libc/ctype.h"

namespace io {

void
ps2::process_scancode(uint8_t keycode)
{
    if (keycode < PS2_SCANCODE_SIZE) {
        char letter = PS2_SCANCODES[keycode][this->state.shift];

        if (letter != 0x0) {
            this->add_char(letter);
            return;
        }
    }

    switch (keycode) {
        /* L Shift */
        case 0x2a:
            this->state.shift = true;
            return;
        case 0xaa:
            this->state.shift = false;
            return;
        /* L Ctrl */
        case 0x1d:
            this->state.ctrl = true;
            return;
        case 0x9d:
            this->state.ctrl = false;
            return;
        /* Alt */
        case 0x38:
            this->state.alt = true;
            return;
        case 0xb8:
            this->state.alt = false;
            return;
        /* Block Mayus */
        case 0x3a:
            this->state.mayus = true;
            return;
        case 0xba:
            this->state.mayus = false;
            return;
        /* Escape */
        case 0x0e:
            this->delete_char(1);
            return;
        case 0x8e:
            return;
    }
}

void
ps2::delete_char(uint16_t n)
{
    if (n > buffer_count)
        return;
    this->buffer_count         = this->buffer_count - n; // -= n volatile deprecated
    this->buffer[buffer_count] = '\0';
    this->has_new_key          = true;
}

void
ps2::add_char(char letter)
{
    if (this->buffer_count > ps2::BUFFER_SIZE - 1)
        return;
    this->buffer[this->buffer_count] = letter;
    this->buffer_count               = this->buffer_count + 1; // += 1 volatile deprecated
    this->has_new_key                = true;
}

bool
ps2::update()
{
    bool temp         = this->has_new_key;
    this->has_new_key = false;
    return temp;
}

const char *
ps2::get_text() const
{
    return this->buffer;
}

} // namespace io
