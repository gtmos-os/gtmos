#include <gtmos/arch.h>
#include <gtmos/kvfs.h>
#include <dev/serial.h>
#include <stddef.h>

void serial_port_write_byte(uint8_t byte) {
    while ((inb(SERIAL_PORT_COM1_BASE + 5) & 0x20) == 0); // Wait for transmit buffer to be empty
    outb(SERIAL_PORT_COM1_BASE, byte);
}

uint8_t serial_port_read_byte() {
    while ((inb(SERIAL_PORT_COM1_BASE + 5) & 0x01) == 0); // Wait for receive buffer to be full
    return inb(SERIAL_PORT_COM1_BASE);
}

void serial_port_write_string(const char* str)
{
    if (str == NULL) {
        return; // Handle null pointer case
    }
    
    while (*str) {
        serial_port_write_byte((uint8_t)(*str)); // Send each character
        str++; // Move to the next character
    }
}