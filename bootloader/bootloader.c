/**
 * Bootloader for UEFI x86_64 machines
 * @info Tested with TianoCore EDK2 Ovmf (make -C toolchain/ edk2)
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "elf/loader.h"
#include "elf/types.h"
#include "codes/values.h"
#include "io/file.h"
#include "log/stdout.h"
#include <uefi.h>

typedef struct
{
    unsigned int *base;
    unsigned long long buffer_size;
    unsigned int width;
    unsigned int height;
    unsigned int ppscl;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct
{
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charsize;
} PSF1_Header;

typedef struct
{
    PSF1_Header *header;
    void *buffer;
} PSF1_Font;

int
main(int argc, char *argv[])
{
    info("started bootloader %s function from %s", __PRETTY_FUNCTION__, __FILE__);
    info("C enviroment is %s", __STDC_HOSTED__ ? "hosted" : "freestanding");
    info("Compilation datetime %s %s", __DATE__, __TIME__);

    if (argc < 2) {
        error("missing kernel file in argv[1]");
        return INCORRECT_ARGC;
    }

    char *memory = (char *)load_file(argv[1]);

	Elf64_Ehdr* elf_header = load_elf(memory);

    /* Getting GOP */
    printf("(I) [bootloader]  initialising Graphics Output Protocol (GOP)\n");
    efi_gop_t *gop      = NULL;
    efi_guid_t gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    efi_status_t status = BS->LocateProtocol(&gop_guid, NULL, (void **)&gop);
    if (gop == NULL || EFI_ERROR(status)) {
        printf("(E) [bootloader]  unable to locate/initialise GOP\n");
    }

    Framebuffer fb;
    fb.base        = (void *)gop->Mode->FrameBufferBase;
    fb.buffer_size = gop->Mode->FrameBufferSize;
    fb.width       = gop->Mode->Information->HorizontalResolution;
    fb.height      = gop->Mode->Information->VerticalResolution;
    fb.ppscl       = gop->Mode->Information->PixelsPerScanLine;

    printf("(I) [bootloader]  GOP base: 0x%x\n", fb.base);
    printf("(I) [bootloader]  GOP buffer size: 0x%x\n", fb.buffer_size);
    printf("(I) [bootloader]  GOP width: %d\n", fb.width);
    printf("(I) [bootloader]  GOP height: %d\n", fb.height);
    printf("(I) [bootloader]  GOP pixels per scan line: %d\n", fb.ppscl);

    /* Load PSF1 font */
    printf("(I) [bootloader]  PSF1 loading font file\n");
    FILE *font_file = fopen("zap-light16.psf", "r");
    /* Move the file pointer to the end */
    fseek(font_file, 0, SEEK_END);
    /* Return current in bytes */
    uint64_t font_size = ftell(font_file);
    /* Move the file pointer to the start */
    fseek(font_file, 0, SEEK_SET);
    void *font_buffer = malloc(font_size);
    fread((void *)font_buffer, font_size, 1, font_file);

    printf("(I) [bootloader]  PSF1 reading header\n");
    PSF1_Header *fhead = (PSF1_Header *)font_buffer;

    if (fhead->magic[0] != PSF1_MAGIC0 || fhead->magic[1] != PSF1_MAGIC1) {
        printf("(E) [bootloader]  PSF1 font incorrect magic header\n");
    } else {
        printf("(I) [bootloader]  PSF1 font correct magic header\n");
    }

    printf("(I) [bootloader]  PSF1 reading glyph data\n");

    /* Assume ignore unicode inf. */
    PSF1_Header *font_glyph = (font_buffer + sizeof(PSF1_Header));

    /* Create the font */
    PSF1_Font *font = malloc(sizeof(PSF1_Font));
    font->header    = fhead;
    font->buffer    = font_glyph;

    printf("(I) [bootloader]  jumping to kernel code at address: 0x%p\n", elf_header->e_entry);

    void (*_start)() =
      ((__attribute__((sysv_abi)) void (*)(Framebuffer *, PSF1_Font *))elf_header->e_entry);

    /* Call kernel */
    _start(&fb, font);

    printf("(I) [bootloader]  returned from kernel\n");

    return 0;
}
