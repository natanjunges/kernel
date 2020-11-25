/* Copyright 2020 Natan Junges

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "boot64.h"

.extern main, stack, .gdt64_code

.global efi64, start64

.section .text
.code64
efi64:
    cli
    mov $stack, %esp
    //Moves multiboot2 special values to final locations
    mov %eax, %edi
    mov %ebx, %esi
start64:
    mov $0, %ax
    mov %ax, %ss
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    call remap_irqs
    call disable_pic
    call enable_lapic
    call fill_idt
    //Loads IDT and enables interrupts
    lidt (.idt64_pointer)
    sti
    call main
.loop:
    hlt
    jmp .loop

/* Remaps the IRQs to interrupts 0x20-0x2F */
remap_irqs:
    mov $(PIC_ICW1_INIT | PIC_ICW1_ICW4), %al
    out %al, $PIC1_COMMAND
    jmp 1f
    1:jmp 2f
    2:
    out %al, $PIC2_COMMAND
    jmp 3f
    3:jmp 4f
    4:
    mov $PIC1_OFFSET, %al
    out %al, $PIC1_DATA
    jmp 5f
    5:jmp 6f
    6:
    mov $PIC2_OFFSET, %al
    out %al, $PIC2_DATA
    jmp 7f
    7:jmp 8f
    8:
    mov $PIC2_MASK, %al
    out %al, $PIC1_DATA
    jmp 9f
    9:jmp 10f
    10:
    mov $PIC2_ID, %al
    out %al, $PIC2_DATA
    jmp 11f
    11:jmp 12f
    12:
    mov $PIC_ICW4_8086, %al
    out %al, $PIC1_DATA
    jmp 13f
    13:jmp 14f
    14:
    out %al, $PIC2_DATA
    jmp 15f
    15:jmp 16f
    16:
    ret

/* Disables PIC by masking all its interrupts */
disable_pic:
    mov $PIC_MASK_ALL, %al
    out %al, $PIC2_DATA
    out %al, $PIC1_DATA
    ret

/* Enables LAPIC by setting the Spurious Interrupt Vector Register */
enable_lapic:
    mov $(LAPIC_ENABLE | LAPIC_SPURIOUS_INTERRUPT), %eax
    mov %eax, (LAPIC_BASE + LAPIC_SIVR)
    ret

fill_idt:
    mov $0, %rcx
    mov $idt64, %rbx
.exc_loop_1:
    mov $exception_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    cmp $16, %rcx
    jne .exc_loop_1
    mov $exception_error_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    mov $exception_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
.exc_err_loop:
    mov $exception_error_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    cmp $30, %rcx
    jne .exc_err_loop
    mov $exception_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    mov $exception_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    mov $exception_error_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
.exc_loop_2:
    mov $exception_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    cmp $60, %rcx
    jne .exc_loop_2
    mov $exception_error_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    mov $exception_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_TRAP), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    mov $0, %rcx
    add $512, %rbx
.pic1_loop:
    mov $pic1_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_INTERRUPT), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    cmp $16, %rcx
    jne .pic1_loop
    mov $0, %rcx
    add $128, %rbx
.pic2_loop:
    mov $pic2_handler, %rax
    movw %ax, (%rbx, %rcx, 8)
    movw $.gdt64_code, 2(%rbx, %rcx, 8)
    movb $0, 4(%rbx, %rcx, 8)
    movb $(IDT_PRESENT | IDT_GATE_TYPE_INTERRUPT), 5(%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
    movl $0, 12(%rbx, %rcx, 8)
    inc %rcx
    inc %rcx
    cmp $16, %rcx
    jne .pic2_loop
    ret

exception_handler:
    iretq

exception_error_handler:
    //Pops the error code
    add $8, %rsp
    iretq

pic1_handler:
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
    mov $PIC_EOI, %al
    out %al, $PIC1_COMMAND
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
    iretq

pic2_handler:
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
    mov $PIC_EOI, %al
    out %al, $PIC2_COMMAND
    out %al, $PIC1_COMMAND
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
    iretq

.section .data
idt64:
    //256 entries, 16 bytes long each
    .space 4096
.idt64_pointer:
    .word . - idt64 - 1
    .quad idt64
