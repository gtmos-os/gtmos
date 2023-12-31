/* Copyright (c) 2016, 2017, 2021 Dennis Wölfing
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* kernel/src/arch/i686/syscall.S
 * Syscall handler.
 */

.section .text
.global syscallHandler
.type syscallHandler, @function
syscallHandler:
    cld

    mov %esp, %ebp
    and $(~0xFF), %esp
    sub $12, %esp
    push %edi

    push %esi
    push %edx
    push %ecx
    push %ebx

    sub $12, %esp
    push %eax

    mov $0x10, %cx
    mov %cx, %ds
    mov %cx, %es

    call getSyscallHandler
    add $16, %esp
    mov __errno_location, %ecx
    movl $0, (%ecx)

    call *%eax
    mov %ebp, %esp

    # Check whether signals are pending.
    mov signalPending, %ecx
    test %ecx, %ecx
    jnz 2f

    mov $0x23, %cx
    mov %cx, %ds
    mov %cx, %es

    mov __errno_location, %ecx
    mov (%ecx), %ecx

1:  iret

    # Fake an InterruptContext so that we can call handleSignal.
2:  sub $8, %esp
    push %ebp
    push %edi
    push %esi
    push %edx
    mov __errno_location, %ecx
    pushl (%ecx)
    push %ebx
    push %eax

    mov %esp, %eax
    and $(~0xFF), %esp # Align the stack
    sub $12, %esp
    push %eax

    call handleSignal
    mov %eax, %esp

    pop %eax
    pop %ebx
    pop %ecx
    pop %edx
    pop %esi
    pop %edi
    pop %ebp
    add $8, %esp

    jmp 1b
.size syscallHandler, . - syscallHandler
