#pragma once

#include "lib/string.h"
#include "stivale2.h"
#include <stdint.h>

static void *
stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id)
{
    stivale2_tag *current_tag = (stivale2_tag *)stivale2_struct->tags;
    for (;;) {
        // If the tag pointer is NULL (end of linked list), we did not find
        // the tag. Return NULL to signal this.
        if (current_tag == nullptr) {
            return nullptr;
        }

        // Check whether the identifier matches. If it does, return a pointer
        // to the matching tag.
        if (current_tag->identifier == id) {
            return current_tag;
        }

        // Get a pointer to the next tag in the linked list and repeat.
        current_tag = (stivale2_tag *)current_tag->next;
    }
}

static void *
stivale2_get_mod(struct stivale2_struct_tag_modules *mod, const char *str)
{
    void *ptr = nullptr;
    for (int i = 0; i < mod->module_count; i++) {
        // TODO: can be changed to strncmp?
        if (strcmp(mod->modules[i].string, str) == 0) {
            ptr = (void *)mod->modules[i].begin;
            break;
        }
    }
    return ptr;
}
