#pragma once

#include <stdint.h>

namespace io {

enum class port
{
    PS2 = 0x60,
};

/* https://wiki.osdev.org/8259_PIC (Initialisation) */
const int PIC1_COMMAND = 0x20;
const int PIC1_DATA    = 0x21;
const int PIC2_COMMAND = 0xA0;
const int PIC2_DATA    = 0xA1;
/* ICW = Initialization Commands Words */
const int ICW1_INIT = 0x10;
const int ICW1_ICW4 = 0x01;
// ICW2 in remap_pic parameters
const int ICW3_PIC1 = 0b00000100;
const int ICW3_PIC2 = 0b00000010;
const int ICW4_8086 = 0x01;

void outb(uint16_t, uint8_t);
void outb(io::port, uint8_t);
uint8_t inb(uint16_t);
uint8_t inb(io::port);
void io_wait();

} // namespace io
