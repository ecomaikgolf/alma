/**
 * Bootloader for UEFI x86_64 machines
 * @info Tested with TianoCore EDK2 OVMF (make -C toolchain/ edk2)
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "bootparams.h"
#include "elf/loader.h"
#include "elf/types.h"
#include "err_values.h"
#include "gop/font.h"
#include "gop/framebuffer.h"
#include "gop/gop.h"
#include "io/file.h"
#include "lib/acpi/acpi.h"
#include "log/stdout.h"
#include "memory/memory.h"
#include <uefi.h>

/**
 * entry function for the UEFI bootloader application
 *
 * @info startup.nsh
 */
int
main(void)
{
    info("started bootloader %s function from %s", __PRETTY_FUNCTION__, __FILE__);
    info("C enviroment is %s", __STDC_HOSTED__ ? "hosted" : "freestanding");
    info("Compilation datetime %s %s", __DATE__, __TIME__);

    /* no argv[1] as real builds don't have the initial EFI script */

    /* startup.nsh kernel parameter */
    /*if (argc < 2) {
        error("missing kernel file in argv[1]");
        return INCORRECT_ARGC;
    }*/

    /* Load the kernel */
    Elf64_Ehdr *elf_header = load_elf("kernel.elf");

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

    /**
     * Load UEFI memory map
     *
     * @warning Moving this function to other line breaks kernel printing
     */
    MapInfo *map = load_memmap();

    /* Obtain the RSDP */
    rsdp_v2 *rsdp = load_rsdp();

    /* Load the PSF1 font */
    PSF1_Font *font = load_psf1_font("zap-light16.psf");

    if (font == NULL) {
        error("font load failure");
        return PSF1_FAILURE;
    }

    /* Check for suspicious values leading to errors */
    if (elf_header->e_entry == 0x0)
        warning("kernel entry point is 0x0");
    if (elf_header->e_entry < 0x100)
        warning("kernel entry point is less than 0x100");

    /* Define the kernel starting function
     * sysv_abi as UEFI uses cdecl for x86 and MS 64-bit convention for x86-64
     */
    void (*_start)() = ((__attribute__((sysv_abi)) void (*)(BootArgs *))elf_header->e_entry);
    info("jumping to kernel code at address: 0x%p", _start);

    info("calling kernel global constructors");

    call_ctors(elf_header);

    info("finished kernel global constructors");

    /* Exit UEFI Boot Services */
    // info("Exiting UEFI Boot Services before the jump");
    // if (exit_bs() > 0) {
    //     error("error exiting UEFI boot services");
    //     return UEFI_BS;
    // }

    /* Call the kernel */
    BootArgs args = { fb, font, map, rsdp };
    _start(&args);

    /* _start shouldn't return */
    while (1) {
    }

    return 0;
}
