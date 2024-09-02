#include <gtmos/hal/panic.h>
#include "./vfs.hpp"

void panic(const char* data) {
    struct flanterm_context *ft_ctx = VFS_GetDisplayContext();

    if (ft_ctx != nullptr)
    {
        const char style[] = "\x1b[1;37;41m";
        flanterm_write(ft_ctx, style, sizeof(style));

        const char line1[] = "\n! KERNEL PANIC OCCURRED !\n";
        flanterm_write(ft_ctx, line1, sizeof(line1));

        flanterm_write(ft_ctx, data, sizeof(data));
    }
}