#![no_std]
#![no_main]
#![feature(abi_x86_interrupt)]
#![feature(const_mut_refs)]

extern crate alloc;

mod serial;
mod arch;
mod task;
mod framebuffer;

use core::panic::PanicInfo;
use core::fmt::Write;

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

use alloc::{boxed::Box, vec, vec::Vec, rc::Rc};

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
    // let mut console = Console::on_frame_buffer(display);
    // console.write_str("Hello World!").ok();
    let heap_value = Box::new(41);
    serial_println!("heap_value at {:p}", heap_value);

    // create a dynamically sized vector
    let mut vec = Vec::new();
    for i in 0..500 {
        vec.push(i);
    }
    serial_println!("vec at {:p}", vec.as_slice());

    // create a reference counted vector -> will be freed when count reaches 0
    let reference_counted = Rc::new(vec![1, 2, 3]);
    let cloned_reference = reference_counted.clone();
    serial_println!("current reference count is {}", Rc::strong_count(&cloned_reference));
    core::mem::drop(reference_counted);
    serial_println!("reference count is {} now", Rc::strong_count(&cloned_reference));

    serial_println!("It did not crash!");
}
