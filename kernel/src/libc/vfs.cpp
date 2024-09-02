#include "./vfs.hpp"
#include <gtmos/io.h>

struct flanterm_context *ft_ctx = nullptr;

void VFS_SetDisplayContext(struct flanterm_context *ft_ctx_in)
{
    ft_ctx = ft_ctx_in;
}

struct flanterm_context *VFS_GetDisplayContext()
{
    return ft_ctx;
}

int VFS_Write(fd_t file, const char data, size_t size)
{
    switch (file)
    {
    case VFS_FD_STDIN:
        return 0;
    case VFS_FD_STDOUT:
    case VFS_FD_STDERR:
        if (ft_ctx == nullptr)
        {
            return -1; // Return an error if ft_ctx is not set
        }
        flanterm_write(ft_ctx, &data, size);
        return size;

    case VFS_FD_DEBUG:
        serial_port_write_byte((uint8_t) data);
        return size;

    default:
        return -1;
    }
}