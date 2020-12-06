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

#include <multiboot2.h>
#include <config.h>

.extern start, efi64

.section .multiboot_header
.align 8
header_start:
    .long MULTIBOOT2_HEADER_MAGIC
    .long MULTIBOOT2_HEADER_ARCHITECTURE
    .long header_end - header_start
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_HEADER_ARCHITECTURE + (header_end - header_start))

.align 8
information_request_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_INFORMATION_REQUEST
    .word MULTIBOOT2_HEADER_TAG_FLAGS_OPTIONAL
    .long information_request_tag_end - information_request_tag_start
    .long MBI_TAG_TYPE_BOOT_COMMAND_LINE
    .long MBI_TAG_TYPE_BOOT_LOADER_NAME
    .long MBI_TAG_TYPE_MODULES
    .long MBI_TAG_TYPE_BASIC_MEMORY_INFORMATION
    .long MBI_TAG_TYPE_MEMORY_MAP
    .long MBI_TAG_TYPE_FRAMEBUFFER_INFO
    .long MBI_TAG_TYPE_ELF_SYMBOLS
    .long MBI_TAG_TYPE_EFI_32_BIT_SYSTEM_TABLE_POINTER
    .long MBI_TAG_TYPE_EFI_64_BIT_SYSTEM_TABLE_POINTER
    .long MBI_TAG_TYPE_SMBIOS_TABLES
    .long MBI_TAG_TYPE_ACPI_OLD_RSDP
    .long MBI_TAG_TYPE_ACPI_NEW_RSDP
    .long MBI_TAG_TYPE_NETWORKING_INFORMATION
    .long MBI_TAG_TYPE_EFI_MEMORY_MAP
    .long MBI_TAG_TYPE_EFI_32_BIT_IMAGE_HANDLE_POINTER
    .long MBI_TAG_TYPE_EFI_64_BIT_IMAGE_HANDLE_POINTER
    .long MBI_TAG_TYPE_IMAGE_LOAD_BASE_PHYSICAL_ADDRESS
information_request_tag_end:

#if FRAMEBUFFER
.align 8
framebuffer_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_FRAMEBUFFER
    .word 0
    .long framebuffer_tag_end - framebuffer_tag_start
    .long 0
    .long 0
    .long 0
framebuffer_tag_end:
#endif/* FRAMEBUFFER*/

#if MODULE_ALIGNMENT
.align 8
module_alignment_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_MODULE_ALIGNMENT
    .word 0
    .long module_alignment_tag_end - module_alignment_tag_start
module_alignment_tag_end:
#endif/* MODULE_ALIGNMENT*/

#if EFI_BOOT_SERVICES
.align 8
efi_boot_services_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_EFI_BOOT_SERVICES
    .word 0
    .long efi_boot_services_tag_end - efi_boot_services_tag_start
efi_boot_services_tag_end:

.align 8
efi_i386_entry_address_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_EFI_I386_ENTRY_ADDRESS
    .word MULTIBOOT2_HEADER_TAG_FLAGS_OPTIONAL
    .long efi_i386_entry_address_tag_end - efi_i386_entry_address_tag_start
    .long start
efi_i386_entry_address_tag_end:

.align 8
efi_amd64_entry_address_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_EFI_AMD64_ENTRY_ADDRESS
    .word MULTIBOOT2_HEADER_TAG_FLAGS_OPTIONAL
    .long efi_amd64_entry_address_tag_end - efi_amd64_entry_address_tag_start
    .long efi64
efi_amd64_entry_address_tag_end:
#endif/* EFI_BOOT_SERVICES*/

#if RELOCATABLE_HEADER
.align 8
relocatable_header_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_RELOCATABLE_HEADER
    .word 0
    .long relocatable_header_tag_end - relocatable_header_tag_start
    .long 0x00100000
    .long 0xFFFFFFFF
    .long 4096
    .long MULTIBOOT2_HEADER_TAG_RELOCATABLE_HEADER_PREFERENCE_NONE
relocatable_header_tag_end:
#endif/* RELOCATABLE_HEADER*/

.align 8
    .word MULTIBOOT2_HEADER_TAG_TYPE_END
    .word 0
    .long 8
header_end:
