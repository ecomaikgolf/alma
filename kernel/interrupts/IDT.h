/**
 * Interrupt Descriptor
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once
#include <stdint.h>

namespace interrupts {

enum class gate_e
{
    interrupt = 0xe,
    trap      = 0xf,
    task      = 0x5,
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
    ~idt_ptr();
} __attribute__((packed));

/**
 * IDT entry
 */
struct idt_entry
{
    uint16_t offset_low;
    uint16_t segment;
    uint16_t bits;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
    idt_entry(uint64_t, uint16_t, uint16_t);
    uint64_t get_offset();
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
