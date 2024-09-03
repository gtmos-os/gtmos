#ifndef __ALLOCATOR_H
#define __ALLOCATOR_H 1

#include <stdint.h>
#include <stddef.h>
#include <dev/efi/efi.h>
#include <utils/bitmap.hpp>

class PageFrameAllocator {
    public:
    void ReadEFIMemoryMap(void* efi_map, size_t efi_map_entries, size_t efi_desc_size);
    Bitmap PageBitmap;
    void FreePage(void* address);
    void FreePages(void* address, uint64_t pageCount);
    void LockPage(void* address);
    void LockPages(void* address, uint64_t pageCount);
    void* RequestPage();
    uint64_t GetFreeRAM();
    uint64_t GetUsedRAM();
    uint64_t GetReservedRAM();


    private:
    void InitBitmap(size_t bitmapSize, void* bufferAddress);
    void ReservePage(void* address);
    void ReservePages(void* address, uint64_t pageCount);
    void UnreservePage(void* address);
    void UnreservePages(void* address, uint64_t pageCount);
};

#endif