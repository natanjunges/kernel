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

.global start, stack, .gdt64_code

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
    //Moves multiboot2 special values to final locations, as %eax and %ebx will be overwritten
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
    //Forces reset by triple fault
    jmp $0xFFFF, $0x0000

check_long_mode:
    mov $CPUID_EXTENDED_FUNCTION, %eax
    cpuid
    cmp $CPUID_EXTENDED_FEATURES, %eax
    jb .no_long_mode
    mov $CPUID_EXTENDED_FEATURES, %eax
    cpuid
    test $CPUID_INTEL_64, %edx
    jz .no_long_mode
    ret
.no_long_mode:
    //Forces reset by triple fault
    jmp $0xFFFF, $0x0000

/* Identity maps the first 4 GiB of memory */
set_up_page_tables:
    mov $p3_table, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov %eax, (p4_table)
    mov $p2_1_table, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov %eax, (p3_table)
    mov $p2_2_table, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov %eax, (p3_table + 8)
    mov $p2_3_table, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov %eax, (p3_table + 16)
    mov $p2_4_table, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT), %eax
    mov %eax, (p3_table + 24)
    mov $0, %ecx
    mov $p2_1_table, %ebx
.map_p2_1_table:
    //2 MiB pages
    mov $0x200000, %eax
    mul %ecx
    or $(PAGING_READ_WRITE | PAGING_PRESENT | PAGING_PAGE_SIZE), %eax
    mov %eax, (%ebx, %ecx, 8)
    inc %ecx
    cmp $512, %ecx
    jne .map_p2_1_table
    mov $0, %ecx
    mov $p2_2_table, %ebx
.map_p2_2_table:
    //2 MiB pages
    mov $0x200000, %eax
    mul %ecx
    add $0x40000000, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT | PAGING_PAGE_SIZE), %eax
    mov %eax, (%ebx, %ecx, 8)
    inc %ecx
    cmp $512, %ecx
    jne .map_p2_2_table
    mov $0, %ecx
    mov $p2_3_table, %ebx
.map_p2_3_table:
    //2 MiB pages
    mov $0x200000, %eax
    mul %ecx
    add $0x80000000, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT | PAGING_PAGE_SIZE), %eax
    mov %eax, (%ebx, %ecx, 8)
    inc %ecx
    cmp $512, %ecx
    jne .map_p2_3_table
    mov $0, %ecx
    mov $p2_4_table, %ebx
.map_p2_4_table:
    //2 MiB pages
    mov $0x200000, %eax
    mul %ecx
    add $0xC0000000, %eax
    or $(PAGING_READ_WRITE | PAGING_PRESENT | PAGING_PAGE_SIZE), %eax
    mov %eax, (%ebx, %ecx, 8)
    inc %ecx
    cmp $512, %ecx
    jne .map_p2_4_table
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
    mov $MSR_IA32_EFER, %ecx
    rdmsr
    or $MSR_IA32_EFER_LME, %eax
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
p2_1_table:
    .space 4096

.align 4096
p2_2_table:
    .space 4096

.align 4096
p2_3_table:
    .space 4096

.align 4096
p2_4_table:
    .space 4096

.align 16
    .space STACK_SIZE
stack:
