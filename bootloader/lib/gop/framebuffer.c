/**
 * Funtionalities to work with the framebuffer
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "gop/framebuffer.h"
#include "log/stdout.h"
#include <uefi.h>

/**
 * Create a framebuffer struct from a GOP structure
 *
 * @param gop GOP to construct from
 * @return Framebuffer
 */
Framebuffer *
create_fb(const efi_gop_t *const gop)
{
    Framebuffer *fb = malloc(sizeof(Framebuffer));

    if (fb == NULL) {
        error("cannot allocate memory for the framebuffer");
        return NULL;
    }

    fb->base        = (char *)gop->Mode->FrameBufferBase;
    fb->buffer_size = gop->Mode->FrameBufferSize;
    fb->height      = gop->Mode->Information->VerticalResolution;
    fb->width       = gop->Mode->Information->HorizontalResolution;
    fb->ppscl       = gop->Mode->Information->PixelsPerScanLine;

    return fb;
}
