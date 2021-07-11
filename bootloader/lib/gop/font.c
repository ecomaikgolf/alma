/**
 * Managing fonts
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "gop/font.h"

/**
 * Load PSF1 font from file
 *
 * @param filename Name of the file
 * @return Font struture
 */
PSF1_Font *
load_psf1_font(const char *const filename)
{
    /* Copy file contents to memory */
    char *memory = load_file(filename);

    /* Not enough memory */
    if (memory == NULL)
        return NULL;

    /* Get PSF1 header struct */
    PSF1_Header *header = get_psf1_header(memory);

    /* Verify PSF1 header */
    uint8_t flags = verify_psf1_header(header);

    /* Incorrect header */
    if (flags > 0)
        return NULL;

    /* Get the glyph font data
     * https://github.com/ecomaikgolf/os-dev/raw/master/toolchain/font/zap-vga16.pdf
     */
    void *glpyh_buffer = get_psf1_glyph(memory);

    /* Create the font structure */
    PSF1_Font *font = malloc(sizeof(PSF1_Font));

    /* Not enough memory */
    if (font == NULL)
        return NULL;

    font->header = header;
    font->buffer = glpyh_buffer;

    return font;
}

/**
 * Get the PSF1 header from a memory block
 *
 * Specification says it's at the start, offset 0
 *
 * @param memory Memory block to read from
 * @return PSF1 Header
 */
PSF1_Header *
get_psf1_header(const char *const memory)
{
    return (PSF1_Header *)memory;
}

/**
 * Verify PSF1 header magic numbers
 *
 * 0x36, 0x04 at the start of the header
 *
 * @param header Header to check
 * @return Flags of the check, 1 if incorrect, 0 if correct
 */
uint8_t
verify_psf1_header(const PSF1_Header *const header)
{
    /* magic[0] == 0x36, magic[1] == 0x04 */
    if (header->magic[0] != PSF1_MAGIC0 || header->magic[1] != PSF1_MAGIC1) {
        error("PSF1 font incorrect magic header");
        return 1;
    } else {
        info("PSF1 font correct magic header");
        return 0;
    }
}

/**
 * Get the PSF1 glyph data from a memory block
 *
 * Specification says it's next to the header
 *
 * @param memory Memory block to read from
 * @return PSF1 glyph data
 */
void *
get_psf1_glyph(const char *const memory)
{
    return (void *)(memory + sizeof(PSF1_Header));
}
