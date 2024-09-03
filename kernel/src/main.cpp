#include <cstdint>
#include <cstddef>
#include <stddef.h>
#include <stdio.h>
#include <gtmos/kvfs.h>
#include <gtmos/arch.h>
#include <dev/efi/efi.h>
#include <dev/memory/memory.hpp>
#include <dev/memory/allocator.hpp>
#include <cstr.h>
#include "./limine.h"

// Set the base revision to 2, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

namespace {

__attribute__((used, section(".requests")))
volatile LIMINE_BASE_REVISION(2);

}

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

namespace {

__attribute__((used, section(".requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_efi_memmap_request efi_memmap_request = {
    .id = LIMINE_EFI_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

}

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .cpp file, as seen fit.

namespace {

__attribute__((used, section(".requests_start_marker")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
volatile LIMINE_REQUESTS_END_MARKER;

}

// The following stubs are required by the Itanium C++ ABI (the one we use,
// regardless of the "Itanium" nomenclature).
// Like the memory functions above, these stubs can be moved to a different .cpp file,
// but should not be removed, unless you know what you are doing.
extern "C" {
    int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
    void __cxa_pure_virtual() { hcf(); }
    void *__dso_handle;
}

// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
extern "C" void kmain()
{
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Call global constructors.
    for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    std::uint32_t *fb_ptr = static_cast<std::uint32_t *>(framebuffer->address);

    struct flanterm_context *ft_ctx = flanterm_fb_init(
        NULL,
        NULL,
        fb_ptr, framebuffer->width, framebuffer->height, framebuffer->pitch,
        framebuffer->red_mask_size, framebuffer->red_mask_shift,
        framebuffer->green_mask_size, framebuffer->green_mask_shift,
        framebuffer->blue_mask_size, framebuffer->blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    );

    VFS_SetDisplayContext(ft_ctx);

    void *efi_memamp = efi_memmap_request.response->memmap;

    if (efi_memamp == nullptr) {
        panic("EFI Memory Map is null!");
    }

    uint64_t efi_mem_entries = efi_memmap_request.response->memmap_size / efi_memmap_request.response->desc_size;
    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    debugf("A");
    PageFrameAllocator newAllocator;
    debugf("B");
    newAllocator.ReadEFIMemoryMap(efi_memamp, efi_mem_entries, efi_memmap_request.response->desc_size);
    debugf("C");
    newAllocator.LockPages(&_KernelStart, kernelPages);

    const char msg[] = "Free RAM %s KB\n";

    printf(msg, to_string(newAllocator.GetFreeRAM() / 1024));
    debugf(msg, to_string(newAllocator.GetFreeRAM() / 1024));

    const char msg1[] = "Used RAM %s KB\n";

    printf(msg1, to_string(newAllocator.GetUsedRAM() / 1024));
    debugf(msg1, to_string(newAllocator.GetUsedRAM() / 1024));

    const char msg2[] = "Reserved  RAM %s KB\n";

    printf(msg2, to_string(newAllocator.GetReservedRAM() / 1024));
    debugf(msg2, to_string(newAllocator.GetReservedRAM() / 1024));

    // We're done, just hang...
    hcf();
}
