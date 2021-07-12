#include "renderer.h"

Renderer::Renderer(Framebuffer *fb,
                   PSF1_Font *font,
                   unsigned int x_offset,
                   unsigned int y_offset,
                   Color color)
{
    this->fb       = fb;
    this->font     = font;
    this->x_offset = x_offset;
    this->y_offset = y_offset;
    this->color    = color;
}

void
Renderer::draw(const char character)
{
    /* Select character from glyph buffer (a character is composed by charsize
     * elements )*/
    char *chr = (char *)this->font->buffer + (character * this->font->header->charsize);

    /* Iterate bitmap "rectangle" (with offset as base) */
    for (unsigned long y = this->y_offset; y < this->y_offset + PSF1_Y; y++) {
        for (unsigned long x = this->x_offset; x < this->x_offset + PSF1_X; x++) {
            /* Each Y from bitmap is a "flag number", check if corresponding
             * x bit is set */
            if ((*chr & (0b10000000 >> (x - this->x_offset)))) {
                *(unsigned int *)(this->fb->base + x + (y * this->fb->ppscl)) =
                  (unsigned int)this->color;
            }
        }
        chr++; /* increase char to iterate over entire charsize elements */
    }
}

void
Renderer::put(const char character)
{
	char addterm[2] = {character, '\0'};
    Renderer::print(addterm);
}

void
Renderer::print(const char *str)
{
    int i = 0;
    while (str[i]) {
        switch (str[i]) {
            case '\n':
                this->y_offset += 16;
                this->x_offset = 0;
                break;
            default: {
                Renderer::draw(str[i]);
                this->x_offset += PSF1_X;
                if (this->x_offset >= this->fb->width) {
                    this->y_offset += PSF1_Y;
                    this->x_offset = 0;
                }
            }
        }
        i++;
    }
}

void
Renderer::println(const char *str)
{
    Renderer::print(str);
    Renderer::put('\n');
}

void
Renderer::setColor(Color color)
{
    this->color = color;
}
