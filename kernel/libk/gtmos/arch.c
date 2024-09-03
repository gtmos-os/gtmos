#include <stdint.h>
#include <stddef.h>
#include <gtmos/arch.h>
#include <gtmos/kvfs.h>

void hcf()
{
    for (;;) {
#if defined (__x86_64__)
        asm ("hlt");
#elif defined (__aarch64__) || defined (__riscv)
        asm ("wfi");
#elif defined (__loongarch64)
        asm ("idle 0");
#endif
    }
}

void panic(const char* data)
{
    struct flanterm_context *ft_ctx = VFS_GetDisplayContext();
    const char style[] = "\x1b[1;37;41m";
    const char line1[] = "\n! KERNEL PANIC OCCURRED !\n";
    const char reset[] = "\x1b[0;0m";

    if (ft_ctx != NULL)
    {
        flanterm_write(ft_ctx, style, sizeof(style));
        flanterm_write(ft_ctx, line1, sizeof(line1));
        flanterm_write(ft_ctx, data, sizeof(data));
        flanterm_write(ft_ctx, reset, sizeof(reset));
    }

    serial_port_write_string(style);
    serial_port_write_string(line1);
    serial_port_write_string(data);
    serial_port_write_string(reset);

    hcf();
}

uint8_t inb(uint16_t port)
{
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

void outb(uint16_t port, uint8_t value)
{
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

