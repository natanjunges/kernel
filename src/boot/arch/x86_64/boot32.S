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

#include <config.h>
#include "boot32.h"

.extern start64

.global start

.section .rodata
gdt64:
    //Null entry
    .quad 0
    .equ .gdt64_code, . - gdt64
    .quad GDT_EXECUTABLE | GDT_DESCRIPTOR_TYPE | GDT_PRESENT | GDT_64_BIT
.gdt64_pointer:
    .word . - gdt64 - 1
    .quad gdt64

.section .text
.code32
start:
    cli
    mov $stack, %esp
    //Move multiboot2 special values to final locations, as %eax and %ebx will be overwritten
    mov %eax, %edi
    mov %ebx, %esi
    call check_cpuid
    call check_long_mode
    call set_up_page_tables
    call load_page_tables
    call enable_pae
    call enter_long_mode
    call enable_paging
    lgdt (.gdt64_pointer)
    jmp $.gdt64_code, $start64

//Prints "ERROR: <%al>" at (0, 0) in EGA text video memory in white with red background
error:
    movw $0x4F45, (0xB8000)
    movw $0x4F52, (0xB8002)
    movw $0x4F52, (0xB8004)
    movw $0x4F4F, (0xB8006)
    movw $0x4F52, (0xB8008)
    movw $0x4F3A, (0xB800A)
    movw $0x4F20, (0xB800C)
    movw $0x4F20, (0xB800E)
    mov %al, (0xB800E)
loop:
    hlt
    jmp loop

check_cpuid:
    pushfl
    pop %eax
    mov %eax, %ecx
    xor $EFLAGS_ID, %eax
    push %eax
    popfl
    pushfl
    pop %eax
    push %ecx
    popfl
    cmp %ecx, %eax
    je .no_cpuid
    ret
.no_cpuid:
    mov $'0', %al
    jmp error

check_long_mode:
    mov $CPUID_EXTENDED_FUNCTIONS, %eax
    cpuid
    cmp $CPUID_EXTENDED_FEATURES, %eax
    jb .no_long_mode
    mov $CPUID_EXTENDED_FEATURES, %eax
    cpuid
    test $CPUID_LONG_MODE, %edx
    jz .no_long_mode
    ret
.no_long_mode:
    mov $'1', %al
    jmp error

/* Identity maps the first 2 MiB of memory */
set_up_page_tables:
    mov $p3_table, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov %eax, (p4_table)
    mov $p2_table, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov %eax, (p3_table)
    mov $p1_table, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov %eax, (p2_table)
    mov $0, %ecx
.map_p1_table:
    //4 KiB pages
    mov $0x1000, %eax
    mul %ecx
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov $p1_table, %ebx
    mov %eax, (%ebx, %ecx, 8)
    inc %ecx
    cmp $512, %ecx
    jne .map_p1_table
    ret

load_page_tables:
    mov $p4_table, %eax
    mov %eax, %cr3
    ret

enable_pae:
    mov %cr4, %eax
    or $CR4_PAE, %eax
    mov %eax, %cr4
    ret

enter_long_mode:
    mov $EFER_MSR, %ecx
    rdmsr
    or $EFER_MSR_LONG_MODE, %eax
    wrmsr
    ret

enable_paging:
    mov %cr0, %eax
    or $CR0_PG, %eax
    mov %eax, %cr0
    ret

.section .bss

.align 4096
p4_table:
    .space 4096

.align 4096
p3_table:
    .space 4096

.align 4096
p2_table:
    .space 4096

.align 4096
p1_table:
    .space 4096

.align 16
    .space STACK_SIZE
stack:
