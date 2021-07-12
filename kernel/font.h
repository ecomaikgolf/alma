const int PSF1_X = 8;
const int PSF1_Y = 16;

struct PSF1_Header
{
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charsize;
};

struct PSF1_Font
{
    PSF1_Header *header;
    void *buffer;
};
