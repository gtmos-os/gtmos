#include <dev/memory/memory.hpp>

uint64_t GetMemorySize(void* efi_map, uint64_t efi_map_entries, uint64_t efi_desc_size)
{
    static uint64_t memorySizeBytes = 0;
    if (memorySizeBytes > 0) return memorySizeBytes;

    for (uint64_t i = 0; i < efi_map_entries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t) efi_map + (i * efi_desc_size));
        memorySizeBytes += desc->numPages * 4096;
    }

    return memorySizeBytes;
}