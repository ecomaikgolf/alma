/**
 * Kernel's Page Frame Allocator
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <stdint.h>

/**
 * x86-64 Virtual Memory Address for paging
 *
 * Yes, addresses are 48bit (reserved field is to do the (address_t *)&addr [addr = uint64_t] trick
 * safely)
 */
struct address_t
{
    uint64_t offset : 12;
    uint64_t table : 9;
    uint64_t mid : 9;
    uint64_t upper : 9;
    uint64_t global : 9;
    uint64_t reserved : 16;
} __attribute__((__packed__));
