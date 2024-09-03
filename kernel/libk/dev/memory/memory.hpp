#ifndef __DEV_MEMORY_H
#define __DEV_MEMORY_H 1

#include <stdint.h>
#include <dev/efi/efi.h>

uint64_t GetMemorySize(void* efi_map, uint64_t efi_map_entries, uint64_t efi_desc_size);

#endif