# GT MOS

## Gallery

| Image | Date | Description |
| ----- | ---- | ----------- |
| ![](./docs/screenshots/Screenshot%202024-09-03%20133058.png) | 3rd September 2024 | Loading the EFI memory map. [*Commit a949eab*](https://github.com/gtmos-os/gtmos/commit/a949eab842dbdbb851c47133f1258933d3e23484) |
| ![](./docs/screenshots/Screenshot%202024-09-02%20195118.png) | 2nd September 2024 | Displaying text on screen *(bottom left)* and input via serial console *(top left)*. [*Commit 6a6b8fc*](https://github.com/gtmos-os/gtmos/commit/6a6b8fc984e46b3224df853a6a41b50c6e3ca93d) |

## How to use this?

### Dependencies

Any `make` command depends on GNU make (`gmake`) and is expected to be run using it. This usually means using `make` on most GNU/Linux distros, or `gmake` on other non-GNU systems.

It is recommended to build this project using a standard UNIX-like system, using a Clang/LLVM toolchain capable of cross compilation (the default, unless `KCC` and/or `KLD` are explicitly set). **(This means you should install `clang` and `lld`) - *unless you know what you are doing.***

Additionally, building an ISO with `make all` requires `xorriso`, and building a HDD/USB image with `make all-hdd` requires `sgdisk` (usually from `gdisk` or `gptfdisk` packages) and `mtools`.

### Architectural targets

The `KARCH` make variable determines the target architecture to build the kernel and image for.

The default `KARCH` is `x86_64`. Other options include: `aarch64`, `loongarch64`, and `riscv64`.

### Makefile targets

Running `make all` will compile the kernel (from the `kernel/` directory) and then generate a bootable ISO image.

Running `make all-hdd` will compile the kernel and then generate a raw image suitable to be flashed onto a USB stick or hard drive/SSD.

Running `make run` will build the kernel and a bootable ISO (equivalent to make all) and then run it using `qemu` (if installed).

Running `make run-hdd` will build the kernel and a raw HDD image (equivalent to make all-hdd) and then run it using `qemu` (if installed).

For x86_64, the `run-bios` and `run-hdd-bios` targets are equivalent to their non `-bios` counterparts except that they boot `qemu` using the default SeaBIOS firmware instead of OVMF.
