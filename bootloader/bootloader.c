/**
 * Bootloader for UEFI x86_64 machines
 * @info Tested with TianoCore EDK2 Ovmf (make -C toolchain/ edk2)
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "codes/values.h"
#include "elf/loader.h"
#include "elf/types.h"
#include "gop/font.h"
#include "gop/framebuffer.h"
#include "gop/gop.h"
#include "io/file.h"
#include "log/stdout.h"
#include <uefi.h>

/**
 * entry function for the UEFI bootloader application
 *
 * @info startup.nsh
 */
int
main(int argc, char *argv[])
{
    info("started bootloader %s function from %s", __PRETTY_FUNCTION__, __FILE__);
    info("C enviroment is %s", __STDC_HOSTED__ ? "hosted" : "freestanding");
    info("Compilation datetime %s %s", __DATE__, __TIME__);

    /* startup.nsh kernel parameter */
    if (argc < 2) {
        error("missing kernel file in argv[1]");
        return INCORRECT_ARGC;
    }

    /* Load the kernel */
    Elf64_Ehdr *elf_header = load_elf(argv[1]);

    if (elf_header == NULL) {
        error("cannot load the kernel");
        return KERNEL_LOAD_FAILURE;
    }

    /* Retrieve the Graphics Output Protocol */
    efi_gop_t *gop = load_gop();

    if (gop == NULL) {
        error("cannot retrieve the gop");
        return GOP_RETRIEVE_FAILURE;
    }

    /* Construct the Framebuffer */
    Framebuffer *fb = create_fb(gop);

    if (gop == NULL) {
        error("cannot construct the framebuffer");
        return FRAMEBUFFER_FAILURE;
    }

    /* Load the PSF1 font */
    PSF1_Font *font = load_psf1_font("zap-light16.psf");

    if (font == NULL) {
        error("font load failure");
        return PSF1_FAILURE;
    }

    if (elf_header->e_entry == 0x0)
        warning("kernel entry point is 0x0");

    if (elf_header->e_entry < 0x100)
        warning("kernel entry point is less than 0x100");

    info("jumping to kernel code at address: 0x%p", elf_header->e_entry);

    /* Define the kernel starting function
     * sysv_abi as UEFI uses cdecl for x86 and MS 64-bit convention for x86-64
     */
    void (*_start)() =
      ((__attribute__((sysv_abi)) void (*)(Framebuffer *, PSF1_Font *))elf_header->e_entry);

    /* Call the kernel */
    _start(fb, font);

    info("returned from the kernel");

    return 0;
}
