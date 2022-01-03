/**
 * Interrupt Descriptor
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once
#include "interrupts.h"
#include <stdint.h>

namespace interrupts {

enum class gate_e
{
    task       = 0x5,
    interrupt  = 0xf,
    trap       = 0x7,
    interrupt2 = 0xe,
    trap2      = 0xf,
};

enum class status_e
{
    enabled  = 0x80,
    disabled = 0x0,
};

enum class vector_e
{
    reserved = 0x9,
};

/**
 * Struct to hold the pointer to the IDT table. We'll use
 * this struct to pass it to the interrupt register
 */
struct idt_ptr
{
    uint16_t lenght;
    uint64_t ptr;
    idt_ptr();
    void set_ptr(uint64_t);
    void add_handle(interrupts::vector_e code, void (*handler)(frame *));
    static void remap_pic(uint8_t, uint8_t);
} __attribute__((packed));

/**
 * IDT entry
 */
struct idt_entry
{
    uint16_t offset_low;
    uint16_t vector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
    uint64_t get_offset();
    void set_offset(uint64_t);
} __attribute__((packed));

namespace helper {

struct addr_t
{
    uint16_t offset_low;
    uint16_t offset_middle;
    uint32_t offset_high;
} __attribute__((packed));

} // namespace helper

/**
 * IDT table
 */
} // namespace interrupts
