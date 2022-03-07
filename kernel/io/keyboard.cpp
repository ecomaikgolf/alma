/**
 * PS2 Keyboard
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "io/keyboard.h"
#include "kernel.h"
#include "lib/ctype.h"

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
            if (this->state == PS2_State::Mayus)
                this->state = PS2_State::Normal;
            else
                this->state = PS2_State::Mayus;
            break;
        case 0xba:
            break;
        /* Del */
        case 0x0e:
            this->delete_char();
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
    this->buffer_count = this->buffer_count - n; // -= n volatile deprecated
    this->has_new_key  = true;
}

void
PS2::add_char(char letter)
{
    /* Surpass buffer handling*/
    if (this->buffer_count >= PS2::buffer_maxsize) {
        switch (this->buffer_handling) {
            case PS2::buffer_mode::circular:
                this->buffer_count = 0;
                break;
            case PS2::buffer_mode::limit:
                return;
        }
    }

    this->buffer[this->buffer_count] = letter;
    this->buffer_count               = this->buffer_count + 1; // += 1 volatile deprecated
    this->has_new_key                = true;
}

bool
PS2::update()
{
    auto temp         = this->has_new_key;
    this->has_new_key = false;
    return temp;
}

void
PS2::scanf(char *buffer, uint32_t maxsize)
{
    if (buffer == nullptr || maxsize <= 0)
        return;

    auto buffer_backup      = this->buffer;
    auto count_backup       = this->buffer_count;
    auto maxsize_backup     = this->buffer_maxsize;
    auto has_new_key_backup = this->has_new_key;
    auto mode_backup        = this->buffer_handling;

    this->buffer          = buffer;
    this->buffer_maxsize  = maxsize - 1;
    this->buffer_count    = 0;
    this->has_new_key     = false;
    this->buffer_handling = PS2::buffer_mode::limit;

    uint32_t last_text_size = 0;
    while (1) {
        if (kernel::keyboard.update()) {
            if (this->buffer_count > 0 && this->buffer[this->buffer_count - 1] == '\n') {
                kernel::tty.newline();
                break;
            }
            auto x_backup     = kernel::tty.get_x();
            auto y_backup     = kernel::tty.get_y();
            auto color_backup = kernel::tty.getColor();
            kernel::tty.setColor(screen::color_e::BLACK);
            for (uint32_t i = 0; i < last_text_size; i++)
                kernel::tty.printchar((char)0xdb);
            kernel::tty.set_x(x_backup);
            kernel::tty.set_y(y_backup);
            kernel::tty.setColor(color_backup);
            auto text                  = this->get_buffer();
            buffer[this->buffer_count] = '\0';

            kernel::tty.print(text);
            last_text_size = this->buffer_count;
            kernel::tty.set_x(x_backup);
            kernel::tty.set_y(y_backup);
        }
    }

    this->buffer[this->buffer_count] = '\0';

    this->buffer          = buffer_backup;
    this->buffer_maxsize  = maxsize_backup;
    this->buffer_count    = count_backup;
    this->has_new_key     = has_new_key_backup;
    this->buffer_handling = mode_backup;
}

void
PS2::enable_keyboard()
{
    kernel::idtr.add_handle(interrupts::vector_e::keyboard, interrupts::keyboard);
}

} // namespace io
