/**
 * Funtionalities for the UEFI GOP
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "gop/framebuffer.h"
#include "log/stdout.h"
#include <uefi.h>

/**
 * Load the GOP
 *
 * @return Pointer to the GOP, NULL if not found
 */
efi_gop_t *
load_gop()
{
    efi_gop_t *gop      = malloc(sizeof(efi_gop_t));

	if(gop == NULL) {
		error("cannot allocate memory for the GOP");
		return NULL;
	}

    efi_guid_t gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    efi_status_t status = BS->LocateProtocol(&gop_guid, NULL, (void **)&gop);

    if (EFI_ERROR(status)) {
        error("unable to initialise GOP");
        return NULL;
    }

    return gop;
}
