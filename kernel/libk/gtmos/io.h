#ifndef __GTMOS_IO_H
#define __GTMOS_IO_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SERIAL_PORT_COM1_BASE 0x3F8

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

void serial_port_write_byte(uint8_t byte);
uint8_t serial_port_read_byte();
void serial_port_write_string(const char* str);

#ifdef __cplusplus
}
#endif

#endif