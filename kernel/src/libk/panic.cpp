#include <gtmos/panic.h>
#include <gtmos/io.h>
#include "../libc/vfs.hpp"

void panic(const char* data)
{
    struct flanterm_context *ft_ctx = VFS_GetDisplayContext();
    const char style[] = "\x1b[1;37;41m";
    const char line1[] = "\n! KERNEL PANIC OCCURRED !\n";
    const char reset[] = "\x1b[0;0m";

    if (ft_ctx != nullptr)
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
}