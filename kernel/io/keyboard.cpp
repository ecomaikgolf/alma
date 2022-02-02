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
    switch (keycode) {
        /* L Shift */
        case 0x2a:
            this->state.shift = true;
            break;
        case 0xaa:
            this->state.shift = false;
            break;
        /* L Ctrl */
        case 0x1d:
            this->state.ctrl = true;
            break;
        case 0x9d:
            this->state.ctrl = false;
            break;
        /* Alt */
        case 0x38:
            this->state.alt = true;
            break;
        case 0xb8:
            this->state.alt = false;
            break;
        /* Block Mayus */
        case 0x3a:
            this->state.mayus = true;
            break;
        case 0xba:
            this->state.mayus = false;
            break;
        /* Escape */
        case 0x0e:
            this->delete_char(1);
            break;
        case 0x8e:
            break;
    }
    if (keycode > PS2_SCANCODE_SIZE)
        return;

    char letter = PS2_SCANCODES[keycode];

    /* Is a valid char */
    if (letter != 0x0) {
        /* Minus */
        if (!(this->state.shift || this->state.mayus))
            letter = tolower(letter);
        /* LShift */
        /* Alt */
        this->add_char(letter);
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
