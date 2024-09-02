#include "./vfs.hpp"

struct flanterm_context *ft_ctx = nullptr;

void VFS_SetDisplayContext(struct flanterm_context *ft_ctx_in)
{
    ft_ctx = ft_ctx_in;
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

    case VFS_FD_DEBUG: // \/ Placeholder implementation for this ...
        if (ft_ctx == nullptr)
        {
            return -1; // Return an error if ft_ctx is not set
        }
        flanterm_write(ft_ctx, &data, size);
        return size;

    default:
        return -1;
    }
}