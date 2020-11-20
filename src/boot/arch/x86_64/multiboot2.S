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

.section .multiboot_header
.align 8
header_start:
    .long MULTIBOOT2_HEADER_MAGIC
    .long MULTIBOOT2_HEADER_ARCHITECTURE_I386
    .long header_end - header_start
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_HEADER_ARCHITECTURE_I386 + (header_end - header_start))

.align 8
information_request_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_INFORMATION_REQUEST
    .word MULTIBOOT2_HEADER_TAG_FLAGS_OPTIONAL
    .long information_request_tag_end - information_request_tag_start
    .long MULTIBOOT2_TAG_TYPE_BOOT_COMMAND_LINE
    .long MULTIBOOT2_TAG_TYPE_BOOT_LOADER_NAME
    .long MULTIBOOT2_TAG_TYPE_MODULES
    .long MULTIBOOT2_TAG_TYPE_BASIC_MEMORY_INFORMATION
    .long MULTIBOOT2_TAG_TYPE_BIOS_BOOT_DEVICE
    .long MULTIBOOT2_TAG_TYPE_MEMORY_MAP
    .long MULTIBOOT2_TAG_TYPE_VBE_INFO
    .long MULTIBOOT2_TAG_TYPE_FRAMEBUFFER_INFO
    .long MULTIBOOT2_TAG_TYPE_ELF_SYMBOLS
    .long MULTIBOOT2_TAG_TYPE_APM_TABLE
    .long MULTIBOOT2_TAG_TYPE_EFI_32_BIT_SYSTEM_TABLE_POINTER
    .long MULTIBOOT2_TAG_TYPE_EFI_64_BIT_SYSTEM_TABLE_POINTER
    .long MULTIBOOT2_TAG_TYPE_SMBIOS_TABLES
    .long MULTIBOOT2_TAG_TYPE_ACPI_OLD_RSDP
    .long MULTIBOOT2_TAG_TYPE_ACPI_NEW_RSDP
    .long MULTIBOOT2_TAG_TYPE_NETWORKING_INFORMATION
    .long MULTIBOOT2_TAG_TYPE_EFI_MEMORY_MAP
    .long MULTIBOOT2_TAG_TYPE_EFI_BOOT_SERVICES_NOT_TERMINATED
    .long MULTIBOOT2_TAG_TYPE_EFI_32_BIT_IMAGE_HANDLE_POINTER
    .long MULTIBOOT2_TAG_TYPE_EFI_64_BIT_IMAGE_HANDLE_POINTER
    .long MULTIBOOT2_TAG_TYPE_IMAGE_LOAD_BASE_PHYSICAL_ADDRESS
information_request_tag_end:

.align 8
framebuffer_tag_start:
    .word MULTIBOOT2_HEADER_TAG_TYPE_FRAMEBUFFER
    .word MULTIBOOT2_HEADER_TAG_FLAGS_OPTIONAL
    .long framebuffer_tag_end - framebuffer_tag_start
    .long 0
    .long 0
    .long 0
framebuffer_tag_end:

.align 8
    .word MULTIBOOT2_HEADER_TAG_TYPE_END
    .word 0
    .long 8
header_end:
