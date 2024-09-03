#ifndef __GTMOS_SERIAL_H
#define __GTMOS_SERIAL_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SERIAL_PORT_COM1_BASE 0x3F8

void serial_port_write_byte(uint8_t byte);
uint8_t serial_port_read_byte();
void serial_port_write_string(const char* str);

#ifdef __cplusplus
}
#endif

#endif