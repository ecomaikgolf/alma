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
print(Framebuffer *fb, PSF1_Font *font,
		const char* str,
		unsigned int x_offset, unsigned int y_offset,
		unsigned int color)
{
	int i = 0;
	while(str[i]) {
		switch(str[i]) {
			case '\n':
				y_offset += 16;
				x_offset = 0;
				break;
			default: {
				  put(fb, font, color, (char)str[i], x_offset, y_offset);
				  x_offset += 8;
				  if(x_offset >= fb->width) {
					  y_offset += 16;
					  x_offset = 0;
				  }
		    }
		}
		i++;
	}
}

void 
_start(Framebuffer *fb, PSF1_Font *font) 
{
	print(fb, font, "Hello world", 10, 10, 0xffffffff);
	return;
}
