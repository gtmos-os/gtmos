#![no_std]
#![no_main]
#![feature(abi_x86_interrupt)]
#![feature(const_mut_refs)]

mod serial;
mod arch;
mod task;
mod framebuffer;

use core::panic::PanicInfo;
use core::fmt::Write;

extern crate alloc;

use arch::x86_64_impl::task::executor::Executor;
use embedded_term::Console;
use framebuffer::Display;
use crate::{arch::x86_64_impl::{self, task::Task}, task::keyboard};

use bootloader_api::{entry_point, BootInfo};
use bootloader_api::config::{BootloaderConfig, Mapping};

use embedded_graphics::{
    draw_target::DrawTarget,
    pixelcolor::{Rgb888, RgbColor},
};

pub static BOOTLOADER_CONFIG: BootloaderConfig = {
    let mut config = BootloaderConfig::new_default();
    config.mappings.physical_memory = Some(Mapping::Dynamic);
    config
};

entry_point!(kernel_main, config = &BOOTLOADER_CONFIG);

fn kernel_main(boot_info: &'static mut BootInfo) -> ! {
    serial_println!("Welcome to GT MOS!");
    use arch::x86_64_impl::allocator;
    use arch::x86_64_impl::memory::{self, BootInfoFrameAllocator};
    use x86_64::VirtAddr;

    x86_64_impl::init();

    if let Some(physical_memory_offset) = boot_info.physical_memory_offset.as_mut() {
        let phys_mem_offset = VirtAddr::new(*physical_memory_offset);
        let mut mapper = unsafe { memory::init(phys_mem_offset) };
        let mut frame_allocator = unsafe { BootInfoFrameAllocator::init(&boot_info.memory_regions) };
        allocator::init_heap(&mut mapper, &mut frame_allocator).expect("heap initialization failed");

        let mut executor = Executor::new();

        if let Some(framebuffer) = boot_info.framebuffer.as_mut() {
            let mut display = framebuffer::Display::new(framebuffer);
            display.clear(Rgb888::BLACK).ok();
            executor.spawn(Task::new(display_task(display)));
        }

        executor.spawn(Task::new(keyboard::print_keypresses()));
        executor.run();
    }
    serial_println!("Kernel Finished - why? ...");
    x86_64_impl::hlt_loop();
}

/// This function is called on panic.
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    serial_println!("{}", info);
    x86_64_impl::hlt_loop();
}

async fn display_task(display: Display) {
    let mut console = Console::on_frame_buffer(display);
    console.write_str("Hello World!").ok();
}
