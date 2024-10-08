# Nuke built-in rules and variables.
MAKEFLAGS += -rR
.SUFFIXES:

# This is the name that our final executable will have.
# Change as needed.
override OUTPUT := kernel

# Convenience macro to reliably declare user overridable variables.
override USER_VARIABLE = $(if $(filter $(origin $(1)),default undefined),$(eval override $(1) := $(2)))

# Target architecture to build for. Default to x86_64.
$(call USER_VARIABLE,KARCH,x86_64)

# User controllable C compiler command.
$(call USER_VARIABLE,KCC,clang)

# User controllable C++ compiler command.
$(call USER_VARIABLE,KCXX,clang++)

# User controllable linker command.
$(call USER_VARIABLE,KLD,ld.lld)

# User controllable C flags.
$(call USER_VARIABLE,KCFLAGS,-g -O2 -pipe)

# User controllable C++ flags. We default to same as C flags.
$(call USER_VARIABLE,KCXXFLAGS,$(KCFLAGS))

# User controllable C/C++ preprocessor flags. We set none by default.
$(call USER_VARIABLE,KCPPFLAGS,)

ifeq ($(KARCH),x86_64)
    # User controllable nasm flags.
    $(call USER_VARIABLE,KNASMFLAGS,-F dwarf -g)
endif

# User controllable linker flags. We set none by default.
$(call USER_VARIABLE,KLDFLAGS,)

# Internal flags shared by both C and C++ compilers.
override SHARED_FLAGS := \
    -Wall \
    -Wextra \
    -nostdinc \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIC \
    -ffunction-sections \
    -fdata-sections

# Internal C/C++ preprocessor flags that should not be changed by the user.
override KCPPFLAGS := \
    -I src \
    $(KCPPFLAGS) \
    -I libc \
    -I libk \
    -I flanterm \
    -I flanterm/backends \
    -MMD \
    -MP

ifeq ($(KARCH),x86_64)
    # Internal nasm flags that should not be changed by the user.
    override KNASMFLAGS += \
        -Wall
endif

# Architecture specific internal flags.
ifeq ($(KARCH),x86_64)
    ifeq ($(KCC),clang)
        override KCC += \
            -target x86_64-unknown-none
    endif
    ifeq ($(KCXX),clang++)
        override KCXX += \
            -target x86_64-unknown-none
    endif
    override SHARED_FLAGS += \
        -m64 \
        -march=x86-64 \
        -mno-80387 \
        -mno-mmx \
        -mno-sse \
        -mno-sse2 \
        -mno-red-zone \
        -mcmodel=kernel
    override KLDFLAGS += \
        -m elf_x86_64
    override KNASMFLAGS += \
        -f elf64
else ifeq ($(KARCH),aarch64)
    ifeq ($(KCC),clang)
        override KCC += \
            -target aarch64-unknown-none
    endif
    ifeq ($(KCXX),clang++)
        override KCXX += \
            -target aarch64-unknown-none
    endif
    override SHARED_FLAGS += \
        -mgeneral-regs-only
    override KLDFLAGS += \
        -m aarch64elf
else ifeq ($(KARCH),riscv64)
    ifeq ($(KCC),clang)
        override KCC += \
            -target riscv64-unknown-none
    endif
    ifeq ($(shell $(KCC) --version | grep -i 'clang'),)
        override KCFLAGS += \
            -march=rv64imac_zicsr_zifencei
    else
        override KCFLAGS += \
            -march=rv64imac
    endif
    ifeq ($(KCXX),clang++)
        override KCXX += \
            -target riscv64-unknown-none
    endif
    ifeq ($(shell $(KCXX) --version | grep -i 'clang'),)
        override KCXXFLAGS += \
            -march=rv64imac_zicsr_zifencei
    else
        override KCXXFLAGS += \
            -march=rv64imac
    endif
    override SHARED_FLAGS += \
        -mabi=lp64 \
        -mno-relax
    override KLDFLAGS += \
        -m elf64lriscv \
        --no-relax
else ifeq ($(KARCH),loongarch64)
    ifeq ($(KCC),clang)
        override KCC += \
            -target loongarch64-unknown-none
    endif
    ifeq ($(KCXX),clang++)
        override KCXX += \
            -target loongarch64-unknown-none
    endif
    override SHARED_FLAGS += \
        -march=loongarch64 \
        -mabi=lp64s
    override KLDFLAGS += \
        -m elf64loongarch \
        --no-relax
else
    $(error Architecture $(KARCH) not supported)
endif

# Internal C flags that should not be changed by the user.
override KCFLAGS += \
    -std=gnu11 \
    $(SHARED_FLAGS)

# Internal C++ flags that should not be changed by the user.
override KCXXFLAGS += \
    -std=gnu++20 \
    -fno-rtti \
    -fno-exceptions \
    $(SHARED_FLAGS)

# Internal linker flags that should not be changed by the user.
override KLDFLAGS += \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -gc-sections \
    -T linker-$(KARCH).ld

# Collect all source files.
override LIBC_CFILES := $(shell find libc -type f -name '*.c' | LC_ALL=C sort)
override LIBK_CFILES := $(shell find libk -type f -name '*.c' | LC_ALL=C sort)
override LIBK_CXXFILES := $(shell find libk -type f -name '*.cpp' | LC_ALL=C sort)
override CFILES := $(shell find src flanterm -type f -name '*.c' | LC_ALL=C sort)
override CXXFILES := $(shell find src flanterm -type f -name '*.cpp' | LC_ALL=C sort)
override ASFILES := $(shell find src flanterm -type f -name '*.S' | LC_ALL=C sort)
ifeq ($(KARCH),x86_64)
override NASMFILES := $(shell find src flanterm -type f -name '*.asm' | LC_ALL=C sort)
endif

# Create object files paths with correct directory structure.
override LIBC_OBJ := $(addprefix obj-$(KARCH)/,$(LIBC_CFILES:.c=.c.o))
override LIBK_OBJ := $(addprefix obj-$(KARCH)/,$(LIBK_CFILES:.c=.c.o) $(LIBK_CXXFILES:.cpp=.cpp.o))
override OBJ := $(addprefix obj-$(KARCH)/,$(CFILES:.c=.c.o) $(CXXFILES:.cpp=.cpp.o) $(ASFILES:.S=.S.o))
ifeq ($(KARCH),x86_64)
override OBJ += $(addprefix obj-$(KARCH)/,$(NASMFILES:.asm=.asm.o))
endif
override HEADER_DEPS := $(addprefix obj-$(KARCH)/,$(LIBK_CFILES:.c=.c.d) $(LIBC_CFILES:.c=.c.d) $(CFILES:.c=.c.d) $(CXXFILES:.cpp:.cpp.d) $(ASFILES:.S=.S.d))

# Ensure the dependencies have been obtained.
override MISSING_DEPS := $(shell if ! test -d flanterm || ! test -f src/cc-runtime.c || ! test -f src/limine.h; then echo 1; fi)
ifeq ($(MISSING_DEPS),1)
    $(error Please run the ./get-deps script first)
endif

# Default target.
.PHONY: all
all: bin-$(KARCH)/$(OUTPUT)

# Link rules for the final executable.
bin-$(KARCH)/$(OUTPUT): GNUmakefile linker-$(KARCH).ld $(LIBC_OBJ) $(LIBK_OBJ) $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(KLD) $(LIBC_OBJ) $(LIBK_OBJ) $(OBJ) $(KLDFLAGS) -o $@

# Include header dependencies.
-include $(HEADER_DEPS)

# Generic compilation rule for all C files.
obj-$(KARCH)/%.c.o: %.c GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

# Generic compilation rule for all C++ files.
obj-$(KARCH)/%.cpp.o: %.cpp GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCXX) $(KCXXFLAGS) $(KCPPFLAGS) -c $< -o $@

# Generic compilation rule for all assembly files.
obj-$(KARCH)/%.S.o: %.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

ifeq ($(KARCH),x86_64)
# Generic compilation rule for all NASM assembly files.
obj-$(KARCH)/%.asm.o: %.asm GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(KNASMFLAGS) $< -o $@
endif

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf bin-$(KARCH) obj-$(KARCH)

# Remove everything built and generated including downloaded dependencies.
.PHONY: distclean
distclean:
	rm -rf bin-* obj-* freestanding-headers freestanding-cxx-hdrs flanterm src/cc-runtime.c src/limine.h

# Install the final built executable to its final on-root location.
.PHONY: install
install: all
	install -d "$(DESTDIR)$(PREFIX)/share/$(OUTPUT)"
	install -m 644 bin-$(KARCH)/$(OUTPUT) "$(DESTDIR)$(PREFIX)/share/$(OUTPUT)/$(OUTPUT)-$(KARCH)"