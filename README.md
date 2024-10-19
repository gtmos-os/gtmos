# GTMOS

A kernel with alot features like GOP and PCI support with uefi bootloader. It has AHCI (The mass storage ATA driver in intel). Also there is a FAT filesystem to read files. So that you can control the ports and drives on your computer. It can handle many interrupts like PS2 Mouse and keyboard. It's also very flexible and all functions are implemented as understandable as they could.

![Screenshot](screenshot.png)

It is a fork of [BAS-kernel](https://github.com/BBjaba/BAS-kernel), but wuth the goal of making a somewhat usable OS.

## dependencies

```
# Ubuntu, Debian:
$ sudo apt install build-essential nasm mtools qemu-system-x86

# Fedora:
$ sudo dnf install gcc gcc-c++ make nasm mtools qemu-system-x86
```
## How to compile

```
$ cd gtmos
$ make
```

```
$ cd gtmos
$ cd gnu-efi
$ make bootloader
$ make
$ cd ..
$ make
```

And keep in mind that if you made any significant changes to the headers in `src` or `desktop` directory, you should delete `lib` folder brefore compilation.
 
## How to run

Keep in mind that if you made any changes to the kernel, you should compile it before running. And also when it ran, wait for the timer to finish in 0.

```
cd gtmos
make run
```

 ## How to modify

 Since some parts of kernel would be hard for beginners, I made another directory named `desktop` that has all important functions included from kernel. You can make changes to it and use any graphics with all supports implemented in kernel.
 
 ## Shell commands

 You can use see all supported commands with using `help` command.
 
 ## implementation progress

- [ ] Kernel:
  - [x] Uefi support
  - [x] GOP for graphics support
  - [x] `.psf` file for font
  - [x] Page frame allocator
  - [x] Interrupts
  - [x] PS2 mouse and keyboard support
  - [x] PCI support
  - [x] AHCI (Drive initialization)
  - [x] AHCI (Port configrations)
  - [x] AHCI (Read from drives)
  - [x] Fat16 and Fat32 support (currentlly broken?)
  - [ ] User mode
  - [ ] Networking drivers
- [ ] Userland:
  - [ ] PE excuteable file support
  - [ ] Adding DE to make a complete OS
    - [ ] ImGUI based GUI toolkit
  - [ ] Some built-in applications

## License

GT MOS is currentlly **All rights reserved**. Some sections may be under a different licesne.

However, some sections of GT MOS come from BAS-Kernel which is self is under the Unlicense licesne [see ./BAS-KERNEL-LICENSE](./BAS-KERNEL-LICENSE).