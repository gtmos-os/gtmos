#ifndef __GTMOS_PANIC_H
#define __GTMOS_PANIC_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

void panic(const char* data);

#ifdef __cplusplus
}
#endif

#endif