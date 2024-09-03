#ifndef __GTMOS_EFI_H
#define __GTMOS_EFI_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct EFI_MEMORY_DESCRIPTOR {
    uint32_t type;
    void* physAddr;
    void* virtAddr; 
    uint64_t numPages;
    uint64_t attribs;
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];

#ifdef __cplusplus
}
#endif

#endif