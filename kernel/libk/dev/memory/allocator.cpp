#include <dev/memory/allocator.hpp>
#include <dev/memory/memory.hpp>
#include <stdio.h>

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Initialized = false;

void PageFrameAllocator::ReadEFIMemoryMap(void* efi_map, size_t efi_map_entries, size_t efi_desc_size)
{
    if (Initialized) return;

    Initialized = true;

    uint64_t efi_mem_entries = efi_map_entries / efi_desc_size;

    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;

    for (uint64_t i = 0; i < efi_mem_entries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t) efi_map + (i * efi_desc_size));
        if (desc->type == 7) // type = EfiConventionalMemory
        {
            if (desc->numPages * 4096 > largestFreeMemSegSize)
            {
                largestFreeMemSeg = desc->physAddr;
                largestFreeMemSegSize = desc->numPages * 4096;
            }
        }
    }

    debugf("B.1");

    uint64_t memorySize = GetMemorySize(efi_map, efi_mem_entries, efi_desc_size);
    debugf("B.2");
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;
    debugf("largestFreeMemSegSize: %i\n", largestFreeMemSegSize);
    debugf("bitmapSize: %i\n", bitmapSize);

    debugf("B.3");

    InitBitmap(bitmapSize, largestFreeMemSeg);

    debugf("B.4");

    LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);

    debugf("B.5");

    for (uint64_t i = 0; i < efi_mem_entries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t) efi_map + (i * efi_desc_size));
        if (desc->type != 7)  // not EfiConventionalMemory
        {
            ReservePages(desc->physAddr, desc->numPages);
        }
    }
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress)
{
    debugf("B.3.1");
    PageBitmap.Size = bitmapSize;
    debugf("B.3.2");
    PageBitmap.Buffer = (uint8_t*) bufferAddress;
    debugf("B.3.3");
    for (uint64_t i = 0; i < bitmapSize; i++)
    {
        debugf("B.3.4");
        *(uint8_t*)(PageBitmap.Buffer + i) = 0;
        debugf("B.3.5");
    }
}

void* PageFrameAllocator::RequestPage()
{
    for (uint64_t index = 0; index < PageBitmap.Size * 8; index++)
    {
        if (PageBitmap[index] == true) continue;
        LockPage((void*)(index * 4096));
        return (void*)(index * 4096);
    }

    return NULL; // Page Frame Swap to file
}

void PageFrameAllocator::FreePage(void* address)
{
    uint64_t index = (uint64_t) address / 4096;
    if (PageBitmap[index] == false) return;
    PageBitmap.Set(index, false);
    freeMemory += 4096;
    usedMemory -= 4096;
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount)
{
    for (uint64_t t = 0; t < pageCount; t++)
    {
        FreePage((void*)((uint64_t) address + (t * 4096)));
    }
}

void PageFrameAllocator::LockPage(void* address)
{
    uint64_t index = (uint64_t) address / 4096;
    if (PageBitmap[index] == true) return;
    PageBitmap.Set(index, true);
    freeMemory -= 4096;
    usedMemory += 4096;
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount)
{
    for (uint64_t t = 0; t < pageCount; t++)
    {
        LockPage((void*)((uint64_t) address + (t * 4096)));
    }
}

void PageFrameAllocator::UnreservePage(void* address)
{
    uint64_t index = (uint64_t) address / 4096;
    if (PageBitmap[index] == false) return;
    PageBitmap.Set(index, false);
    freeMemory += 4096;
    reservedMemory -= 4096;
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount)
{
    for (uint64_t t = 0; t < pageCount; t++)
    {
        UnreservePage((void*)((uint64_t) address + (t * 4096)));
    }
}

void PageFrameAllocator::ReservePage(void* address)
{
    uint64_t index = (uint64_t) address / 4096;
    if (PageBitmap[index] == true) return;
    PageBitmap.Set(index, true);
    freeMemory -= 4096;
    reservedMemory += 4096;
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount)
{
    for (uint64_t t = 0; t < pageCount; t++)
    {
        ReservePage((void*)((uint64_t) address + (t * 4096)));
    }
}

uint64_t PageFrameAllocator::GetFreeRAM()
{
    return freeMemory;
}

uint64_t PageFrameAllocator::GetUsedRAM()
{
    return usedMemory;
}

uint64_t PageFrameAllocator::GetReservedRAM()
{
    return reservedMemory;
}