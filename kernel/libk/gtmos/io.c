#include <stdint.h>
#include <stddef.h>
#include <gtmos/io.h>

uint8_t inb(uint16_t port) {
#if defined(__x86_64__)
    uint8_t value;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
#elif defined(__aarch64__)
    uint8_t value;
    __asm__ __volatile__ ("ldr b0, [x0, #0]" : "=r"(value) : "r"(port));
    return value;
#elif defined(__riscv)
    uint8_t value;
    __asm__ __volatile__ ("lb %0, 0(%1)" : "=r"(value) : "r"(port));
    return value;
#elif defined(__loongarch64)
    uint8_t value;
    __asm__ __volatile__ ("ld %0, 0(%1)" : "=r"(value) : "r"(port));
    return value;
#else
    #error "Unsupported architecture"
#endif
}

void outb(uint16_t port, uint8_t value) {
#if defined(__x86_64__)
    __asm__ __volatile__ ("outb %0, %1" : : "a"(value), "Nd"(port));
#elif defined(__aarch64__)
    __asm__ __volatile__ ("strb %0, [x1, #0]" : : "r"(value), "r"(port));
#elif defined(__riscv)
    __asm__ __volatile__ ("sb %0, 0(%1)" : : "r"(value), "r"(port));
#elif defined(__loongarch64)
    __asm__ __volatile__ ("st %0, 0(%1)" : : "r"(value), "r"(port));
#else
    #error "Unsupported architecture"
#endif
}

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