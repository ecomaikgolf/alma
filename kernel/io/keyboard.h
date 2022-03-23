#pragma once

/**
 * PS2 Keyboard
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include <stdint.h>

namespace io {

/**
 * Scancode mapping for a ES keyboard with 1 byte chars
 */
const char PS2_SCANCODES[][4] = {
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { '1', '!', '|', '1' },
    { '2', '"', '@', '2' },   { '3', 0x0, '#', '3' },     { '4', '$', '~', '4' },
    { '5', '%', 0x0, '5' },   { '6', '&', 0x0, '6' },     { '7', '/', '}', '7' },
    { '8', '(', '[', '8' },   { '9', ')', ']', '9' },     { '0', '=', '}', '0' },
    { '\'', '?', '\\', 0x0 }, { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 'q', 'Q', 0x0, 'Q' },     { 'w', 'W', 0x0, 'W' },
    { 'e', 'E', 0x0, 'E' },   { 'r', 'R', 0x0, 'R' },     { 't', 'T', 0x0, 'T' },
    { 'y', 'Y', 0x0, 'Y' },   { 'u', 'U', 0x0, 'U' },     { 'i', 'I', 0x0, 'I' },
    { 'o', 'O', 0x0, 'O' },   { 'p', 'P', 0x0, 'P' },     { '`', '^', '[', 0x0 },
    { '+', '*', ']', 0x0 },   { '\n', '\n', '\n', '\n' }, { 0x0, 0x0, 0x0, 0x0 },
    { 'a', 'A', 0x0, 'A' },   { 's', 'S', 0x0, 'S' },     { 'd', 'D', 0x0, 'D' },
    { 'f', 'F', 0x0, 'F' },   { 'g', 'G', 0x0, 'G' },     { 'h', 'H', 0x0, 'H' },
    { 'j', 'J', 0x0, 'J' },   { 'k', 'K', 0x0, 'K' },     { 'l', 'L', 0x0, 'L' },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 'z', 'Z', 0x0, 'Z' },
    { 'x', 'X', 0x0, 'X' },   { 'c', 'C', 0x0, 'C' },     { 'v', 'V', 0x0, 'V' },
    { 'b', 'B', 0x0, 'B' },   { 'n', 'N', 0x0, 'N' },     { 'm', 'M', 0x0, 'M' },
    { ',', ';', 0x0, 0x0 },   { '.', ':', 0x0, 0x0 },     { '-', '_', 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { ' ', ' ', 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { 0x0, 0x0, 0x0, 0x0 },
    { 0x0, 0x0, 0x0, 0x0 },   { 0x0, 0x0, 0x0, 0x0 },     { '<', '>', 0x0, 0x0 },
};

/** Ammount of keys in the mapping */
const unsigned int PS2_SCANCODE_SIZE = sizeof(PS2_SCANCODES) / sizeof(char[4]);

/** States of the keyboard */
enum class PS2_State
{
    Normal  = 0,
    Shifted = 1,
    Alt     = 2,
    Mayus   = 3,
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
class PS2
{
  public:
    /** Process a new scancode */
    void process_scancode(uint8_t);
    /** Delete n chars from buffer */
    void delete_char(uint16_t n = 1);
    /** Add a new char */
    void add_char(char);
    /** Add a new mod char */
    void add_modchar(char);
    /** True if we need to update keyboard values */
    bool update();
    /** Read */
    void scanf(char *, uint32_t);
    /** Returns the buffered input text */
    const char *get_buffer() const
    {
        return this->buffer;
    };
    /** Sets a new buffer to store text */
    void set_buffer(char *buffer)
    {
        this->buffer = buffer;
    };
    uint16_t get_maxsize() const
    {
        return this->buffer_maxsize;
    };
    void set_maxsize(uint16_t maxsize)
    {
        this->buffer_maxsize = maxsize;
    };
    static void enable_keyboard();

  private:
    const static uint16_t DEFAULT_MAXSIZE = 256;
    /** Input buffer max size */
    uint16_t buffer_maxsize = DEFAULT_MAXSIZE;
    /** Chars in the input buffer */
    volatile uint16_t buffer_count = 0;
    /** Input buffer */
    char *buffer = nullptr;
    /** Buffer has unrequested changes */
    bool has_new_key = false;
    /** Keyboard state */
    PS2_State state = PS2_State::Normal;

    enum class buffer_mode
    {
        circular,
        limit
    };

    enum class read_mode
    {
        kernel,
        scanf,
    };

    buffer_mode buffer_handling;
    volatile read_mode input_mode = read_mode::kernel;

    void update_scanf();
};

} // namespace io
