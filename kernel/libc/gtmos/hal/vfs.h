#ifndef __GTMOS_VFS_H
#define __GTMOS_VFS_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef int fd_t;

#define VFS_FD_STDIN    0
#define VFS_FD_STDOUT   1
#define VFS_FD_STDERR   2
#define VFS_FD_DEBUG    3

int VFS_Write(fd_t file, uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif