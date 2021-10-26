/**
 * Class to output to the screen
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "screen/renderer.h"

// screen::renderer_i *global;

namespace screen {

/**
 * Draw a character to the screen
 *
 * Increases x_ffset and y_offset, also recognises special chars as '\n'
 *
 * @param character Char to print
 */
void
renderer_i::put(const char character)
{
    char aux[2] = { character, '\0' };
    this->print(aux);
}

/**
 * Print a string without default newline
 *
 * Increases x_offset and y_offset, also recognises special chars as '\n'
 *
 * @param str string to print
 */
void
renderer_i::print(const char *str)
{
    int i = 0;
    while (str[i]) {
        switch (str[i]) {
            case '\n':
                this->y_offset += this->glyph_y();
                this->x_offset = 0;
                break;
            default: {
                this->draw(str[i]);
                this->x_offset += this->glyph_x();
                if (this->x_offset >= this->fb->width) {
                    this->y_offset += this->glyph_y();
                    this->x_offset = 0;
                }
            }
        }
        i++;
    }
}

/**
 * Print a string with a default newline
 *
 * Increases x_offset and y_offset, also recognises special chars as '\n'
 *
 * @param str string to print
 */
void
renderer_i::println(const char *str)
{
    this->print(str);
    this->put('\n');
}

/**
 * Sets a new char color to use
 *
 * @param color Color to use onwards
 */
void
renderer_i::setColor(color_e color)
{
    this->color = color;
}

} // namespace screen
