/**
 * Color definitions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

namespace screen {

/**
 * Color enum. Colors are hex values
 *
 * @see https://en.wikipedia.org/wiki/Web_colors
 */
enum class color_e
{
    WHITE   = 0xFFFFFF,
    SILVER  = 0xC0C0C0,
    GRAY    = 0x808080,
    BLACK   = 0x000000,
    RED     = 0xFF0000,
    MAROON  = 0x800000,
    YELLOW  = 0xFFFF00,
    OLIVE   = 0x808000,
    LIME    = 0x00FF00,
    GREEN   = 0x008000,
    AQUA    = 0x00FFFF,
    TEAL    = 0x008080,
    BLUE    = 0x0000FF,
    NAVY    = 0x000080,
    FUCHSIA = 0xFF00FF,
    PURPLE  = 0x800080,
};

} // namespace screen
