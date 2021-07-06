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

void _start(Framebuffer *fb, PSF1_Font *font) {

	return;
}
