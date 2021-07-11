/**
 * Functions to manage files in a UEFI enviroment using posix-uefi wrappers
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "io/file.h"
#include "log/stdout.h"
#include <uefi.h>

/**
 * Wrapper to return file size
 * @invariant Does not modify file read pointer
 *
 * @param file File input
 * @return File size in bytes
 */
uint64_t
file_size(FILE *file)
{
    if (file == NULL) {
        warning("file parameter is NULL");
        return 0;
    }

    /* Store file read pointer */
    uint64_t initial = ftell(file);
    /* Move the file read pointer to the end */
    fseek(file, 0, SEEK_END);
    /* Return current in bytes */
    uint64_t size = ftell(file);
    /* Move the file reda pointer to the intial position*/
    fseek(file, initial, SEEK_SET);

    return size;
}

/**
 * Load file to memory from filename
 * @info Does not modify file read pointer
 *
 * @param file Filename
 * @return Pointer to memory or null if any error occurr
 */
void *
load_file(const char *filename)
{
    info("opening '%s' file", filename);

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        error("%s could not be opened", filename);
        return NULL;
    }

    info("%s file opened", filename);

    /* get file size to alloc enough space */
    uint64_t size = file_size(file);

    /* allocate memory for file contents */
    void *memory = malloc(size);

    if (memory == NULL) {
        error("couldn't allocate memory for %s", filename);
        return NULL;
    }

    info("allocated %d bytes for %s contents starting in 0x%p", size, filename, memory);

    /* Copy file contents to memory */
    fread((void *)memory, size, 1, file);
    info("copied %s contents to memory 0x%p (%d bytes)", filename, memory, size);

    // fclose(file);
    info("closed %s", filename);

    return memory;
}
