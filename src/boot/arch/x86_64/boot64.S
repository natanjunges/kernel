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

.extern main, stack, .gdt64_code, interrupt_handlers

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
    mov $PIC1_MASK_ALL, %al
    out %al, $PIC1_DATA
    mov $PIC2_MASK_ALL, %al
    out %al, $PIC2_DATA
    ret

.macro IDT64_FILL_ENTRY vector
    mov $\vector, %rcx
    shl $1, %rcx
    mov $irq_\vector, %rax
    movw %ax, (%rbx, %rcx, 8)
    shr $16, %rax
    movw %ax, 6(%rbx, %rcx, 8)
    shr $16, %rax
    movl %eax, 8(%rbx, %rcx, 8)
.endm

fill_idt:
    mov $idt64, %rbx
    .altmacro
    .set i, 0
    .rept 256
        IDT64_FILL_ENTRY %i
        .set i, i + 1
    .endr
    .noaltmacro
    ret

interrupt_handler:
    cli
    push %rax
    mov 16(%rsp), %rax
    push %rbx
    mov 16(%rsp), %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
    mov %rbx, %rdi
    mov %rax, %rsi
    mov %rbx, %rcx
    mov $interrupt_handlers, %rbx
    mov (%rbx, %rcx, 8), %rax
    call *%rax
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
    add $16, %rsp
    sti
    iretq

.macro EXC_ERR_HANDLER vector
irq_\vector:
    push $\vector
    jmp interrupt_handler
.endm

.macro INT_HANDLER vector
irq_\vector:
    push $0
    push $\vector
    jmp interrupt_handler
.endm

.macro EXC_NO_ERR_HANDLER vector
INT_HANDLER \vector
.endm

EXC_NO_ERR_HANDLER 0

EXC_NO_ERR_HANDLER 1

EXC_NO_ERR_HANDLER 2

EXC_NO_ERR_HANDLER 3

EXC_NO_ERR_HANDLER 4

EXC_NO_ERR_HANDLER 5

EXC_NO_ERR_HANDLER 6

EXC_NO_ERR_HANDLER 7

EXC_ERR_HANDLER 8

EXC_NO_ERR_HANDLER 9

EXC_ERR_HANDLER 10

EXC_ERR_HANDLER 11

EXC_ERR_HANDLER 12

EXC_ERR_HANDLER 13

EXC_ERR_HANDLER 14

EXC_NO_ERR_HANDLER 15

EXC_NO_ERR_HANDLER 16

EXC_ERR_HANDLER 17

EXC_NO_ERR_HANDLER 18

EXC_NO_ERR_HANDLER 19

EXC_NO_ERR_HANDLER 20

EXC_NO_ERR_HANDLER 21

EXC_NO_ERR_HANDLER 22

EXC_NO_ERR_HANDLER 23

EXC_NO_ERR_HANDLER 24

EXC_NO_ERR_HANDLER 25

EXC_NO_ERR_HANDLER 26

EXC_NO_ERR_HANDLER 27

EXC_NO_ERR_HANDLER 28

EXC_NO_ERR_HANDLER 29

EXC_ERR_HANDLER 30

EXC_NO_ERR_HANDLER 31

.altmacro
.set i, 32
.rept 224
    INT_HANDLER %i
    .set i, i + 1
.endr
.noaltmacro

.macro IDT64_EXC_ENTRY vector
    .word 0
    .word .gdt64_code
    .byte 0
    .byte IDT_PRESENT | IDT_GATE_TYPE_TRAP
    .word 0
    .long 0
    .long 0
.endm

.macro IDT64_INT_ENTRY vector
    .word 0
    .word .gdt64_code
    .byte 0
    .byte IDT_PRESENT | IDT_GATE_TYPE_INTERRUPT
    .word 0
    .long 0
    .long 0
.endm

.section .data
idt64:
    //256 entries, 16 bytes long each
    .altmacro
    .set i, 0
    .rept 32
        IDT64_EXC_ENTRY %i
        .set i, i + 1
    .endr
    .noaltmacro

    .altmacro
    .set i, 32
    .rept 224
        IDT64_INT_ENTRY %i
        .set i, i + 1
    .endr
    .noaltmacro
.idt64_pointer:
    .word . - idt64 - 1
    .quad idt64
