/**
 * PS2 Keyboard
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "io/keyboard.h"
#include "kernel.h"
#include "libc/ctype.h"

namespace io {

void
PS2::process_scancode(uint8_t keycode)
{
    if (keycode < PS2_SCANCODE_SIZE) {
        char letter = PS2_SCANCODES[keycode][static_cast<int>(this->state)];

        if (letter != 0x0) {
            this->add_char(letter);
            return;
        }
    }

    switch (keycode) {
        /* L Shift */
        case 0x2a:
            this->state = PS2_State::Shifted;
            break;
        case 0xaa:
            this->state = PS2_State::Normal;
            break;
        /* L Ctrl */
        case 0x1d:
            break;
        case 0x9d:
            break;
        /* Alt */
        case 0x38:
            this->state = PS2_State::Alt;
            break;
        case 0xb8:
            this->state = PS2_State::Normal;
            break;
        /* Block Mayus */
        case 0x3a:
            if (this->state == PS2_State::Mayus)
                this->state = PS2_State::Normal;
            else
                this->state = PS2_State::Mayus;
            break;
        case 0xba:
            break;
        /* Del */
        case 0x0e:
            this->delete_char(1);
            break;
        case 0x8e:
            break;
    }
}

void
PS2::delete_char(uint16_t n)
{
    if (n > buffer_count)
        return;
    this->buffer_count         = this->buffer_count - n; // -= n volatile deprecated
    this->buffer[buffer_count] = '\0';
    this->has_new_key          = true;
}

void
PS2::add_char(char letter)
{
    /* Infinite circular buffer */
    if (this->buffer_count >= PS2::buffer_maxsize)
        this->buffer_count = 0;
    this->buffer[this->buffer_count] = letter;
    this->buffer_count               = this->buffer_count + 1; // += 1 volatile deprecated
    this->buffer[this->buffer_count] = '\0';
    this->has_new_key                = true;
}

bool
PS2::update()
{
    bool temp         = this->has_new_key;
    this->has_new_key = false;
    return temp;
}

const char *
PS2::get_text() const
{
    return this->buffer;
}

void
PS2::scanf(char *buffer, uint32_t maxsize)
{
    auto buffer_backup      = this->buffer;
    auto count_backup       = this->buffer_count;
    auto maxsize_backup     = this->buffer_maxsize;
    auto has_new_key_backup = this->has_new_key;

    this->buffer         = buffer;
    this->buffer_maxsize = maxsize;
    this->buffer_count   = 0;
    this->has_new_key    = false;

    uint32_t last_text_size = 0;
    while (1) {
        if (kernel::keyboard.update()) {
            if (this->buffer_count > 0 && this->buffer[this->buffer_count - 1] == '\n') {
                this->buffer[this->buffer_count] = '\0';
                kernel::tty.println("");
                break;
            }

            auto x_backup = kernel::tty.get_x();
            auto y_backup = kernel::tty.get_y();
            char aux[]    = { (char)0xdb, '\0' };
            kernel::tty.setColor(screen::color_e::BLACK);
            for (int i = 0; i < last_text_size; i++)
                kernel::tty.print(aux);
            kernel::tty.setColor(screen::color_e::WHITE);
            kernel::tty.set_x(x_backup);
            kernel::tty.set_y(y_backup);
            auto text = this->get_text();
            kernel::tty.print(text);
            last_text_size = this->buffer_count;
            kernel::tty.set_x(x_backup);
            kernel::tty.set_y(y_backup);
        }
    }

    this->buffer         = buffer_backup;
    this->buffer_maxsize = maxsize_backup;
    this->buffer_count   = count_backup;
    this->has_new_key    = has_new_key_backup;
}

} // namespace io
