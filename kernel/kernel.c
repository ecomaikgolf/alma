typedef struct {
	unsigned int* base;
	unsigned long long buffer_size;
	unsigned int width;
	unsigned int height;
	unsigned int ppscl;
} Framebuffer;

typedef struct {
  unsigned char magic[2];
  unsigned char mode;
  unsigned char charsize;
} PSF1_Header;

typedef struct {
  PSF1_Header* header;
  void* buffer;
} PSF1_Font;

void 
put(Framebuffer *fb, PSF1_Font *font,
		unsigned int color, char character,
		unsigned int x_offset, unsigned int y_offset) 
{
	/* Select character from glyph buffer (a character is composed by charsize elements )*/
	char* chr = (char *)font->buffer + (character * font->header->charsize);

	/* Iterate bitmap "rectangle" (with offset as base) */
	for(unsigned long y = y_offset ; y < y_offset + 16 ; y++) {
		for(unsigned long x = x_offset ; x < x_offset + 8 ; x++) {
			/* Each Y from bitmap is a "flag number", check if corresponding
			 * x bit is set */
			if((*chr & (0b10000000 >> (x - x_offset)))) {
				*(unsigned int*)(fb->base + x + (y * fb->ppscl)) = color;
			} 
		}
		chr++; /* increase char to iterate over entire charsize elements */
	}
}

void 
_start(Framebuffer *fb, PSF1_Font *font) 
{
	put(fb, font, 0xffffffff, 'E', 10, 10);
	return;
}
