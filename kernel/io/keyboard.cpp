/**
 * PS2 Keyboard
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "io/keyboard.h"
#include "kernel.h"
#include "lib/ctype.h"
#include "lib/math.h"

namespace io {

/**
 * Process a PS2 keycode
 *
 * Manages some modkeys, mayus, 8bit chars. Pushes chars into a kernel buffer (circular mode)
 */
void
PS2::process_scancode(uint8_t keycode)
{
    /* keycode received inside char map */
    if (keycode < PS2_SCANCODE_SIZE) {
        char letter = PS2_SCANCODES[keycode][static_cast<int>(this->state)];

        if (letter != 0x0) {
            /* User entered a char */
            this->add_char(letter);
            /* Special hardwire to scanf buffer to improve performance */
            if (this->input_mode == PS2::read_mode::scanf)
                this->update_scanf();
            return;
        } // else unsuported char OR modkey
    }

    /* Handle modkeys */
    switch (keycode) {
        /* L Shift */
        case 0x2a:
            this->state = PS2_State::Shifted;
            break;
        case 0xaa:
            if (this->state != PS2_State::Mayus)
                this->state = PS2_State::Normal;
            break;
        case 0x3a:
            if (this->state == PS2_State::Mayus)
                this->state = PS2_State::Normal;
            else
                this->state = PS2_State::Mayus;
            break;
        case 0xba:
            break;
        /* Del */
        case 0x0e: {
            this->delete_char();
            /* Special hardwire to scanf buffer to improve performance */
            if (this->input_mode == PS2::read_mode::scanf)
                this->update_scanf();
            break;
        }
        case 0x8e:
            break;
    }
}

/**
 * Delete N chars from the keyboard buffer
 *
 * default = 1
 */
void
PS2::delete_char(uint16_t n)
{
    if (n > buffer_count)
        return;
    this->buffer_count = this->buffer_count - n; // -= n volatile deprecated
    this->has_new_key  = true;
}

/**
 * Add a new char to the keyboard buffer
 */
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

/**
 * Check for keyboard buffe rupdates
 */
bool
PS2::update()
{
    auto temp         = this->has_new_key;
    this->has_new_key = false;
    return temp;
}

/**
 * Ordinary scanf function
 *
 * Substitutes the keyboard buffer in order to improve performance and avoid unnecessary copies
 */
void
PS2::scanf(char *buffer, uint32_t maxsize)
{
    if (buffer == nullptr || maxsize <= 0)
        return;

    // TODO: Reformat with a copy constructor (we don't copy everything) or a function
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
    this->input_mode      = read_mode::scanf;

    /* Wait until user finished introducing text */
    while (this->input_mode == read_mode::scanf) {
    }

    this->buffer[this->buffer_count] = '\0';
    kernel::tty.newline();

    /* Restore kernel buffer */
    this->buffer          = buffer_backup;
    this->buffer_maxsize  = maxsize_backup;
    this->buffer_count    = count_backup;
    this->has_new_key     = has_new_key_backup;
    this->buffer_handling = mode_backup;
}

/**
 * Scanf under the hood
 *
 * Responsible to manage the screen while executing scanf(). This function ins called from
 * process_scancode() directly from the interrupt to improve performance
 */
void
PS2::update_scanf()
{
    static uint32_t last_text_size = 0;
    if (this->buffer_count > 0 && this->buffer[this->buffer_count - 1] == '\n') {
        /* User ends scanf with enter */
        this->input_mode = read_mode::kernel;
        last_text_size   = 0;
        return;
    } else if (this->buffer_count > last_text_size) {
        /* User enters new character(s) */
        auto new_chars             = this->buffer_count - last_text_size;
        buffer[this->buffer_count] = '\0';
        kernel::tty.print(&this->buffer[last_text_size]);
    } else if (this->buffer_count < last_text_size) {
        /* User removes character(s) */
        auto del_chars = last_text_size - this->buffer_count;
        int new_x      = kernel::tty.get_x() - (del_chars * kernel::tty.glyph_x());
        if (new_x < 0) [[unlikely]] {
            kernel::tty.set_x(kernel::tty.get_width() - abs(new_x));
            kernel::tty.set_y(kernel::tty.get_y() - kernel::tty.glyph_y());
        } else [[likely]] {
            kernel::tty.set_x(new_x);
        }
        kernel::tty.pushCoords(kernel::tty.get_x(), kernel::tty.get_y());
        kernel::tty.pushColor(screen::color_e::BLACK);
        for (uint32_t i = 0; i < del_chars; i++)
            kernel::tty.put((char)0xdb); // Full color character
        kernel::tty.popColor();
        kernel::tty.popCoords();
    }
    last_text_size = this->buffer_count;
}

/**
 * Enables the keyboard by mapping the PS2 interrupt with the keyboard interrupt function
 */
void
PS2::enable_keyboard()
{
    kernel::idtr.add_handle(interrupts::vector_e::keyboard, interrupts::keyboard);
}

} // namespace io
