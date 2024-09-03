#ifndef _VFS_H
#define _VFS_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <gtmos/hal/vfs.h>

#include "../../flanterm/backends/fb.h"
#include "../../flanterm/flanterm.h"

void VFS_SetDisplayContext(struct flanterm_context *ft_ctx_in);

struct flanterm_context *VFS_GetDisplayContext();

#ifdef __cplusplus
}
#endif

#endif