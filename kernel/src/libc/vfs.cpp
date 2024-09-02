#include "./vfs.hpp"

struct flanterm_context *ft_ctx;

void VFS_SetDisplayContext(struct flanterm_context *ft_ctx_in)
{
    ft_ctx = ft_ctx_in;
}

int VFS_Write(fd_t file, uint8_t* data, size_t size)
{
    switch (file)
    {
    case VFS_FD_STDIN:
        return 0;
    case VFS_FD_STDOUT:
    case VFS_FD_STDERR:
        for (size_t i = 0; i < size; i++)
            ft_ctx->raw_putchar(ft_ctx, data[i]);
        return size;

    case VFS_FD_DEBUG:
        for (size_t i = 0; i < size; i++)
            ft_ctx->raw_putchar(ft_ctx, data[i]);
        return size;

    default:
        return -1;
    }
}