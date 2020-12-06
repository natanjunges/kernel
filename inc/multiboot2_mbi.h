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

#ifndef MULTIBOOT2_MBI_H
#define MULTIBOOT2_MBI_H

#include <multiboot2.h>
#include <acpi.h>
#include <stdint.h>

struct mbi_tag {
    uint32_t type;
    uint32_t size;
} __attribute__((packed));

struct mbi_header {
    uint32_t total_size;
    uint32_t reserved;
    struct mbi_tag tags[0];
} __attribute__((packed));

struct mbi_tag_boot_command_line {
    struct mbi_tag header;
    uint8_t string[0];
} __attribute__((packed));

struct mbi_tag_boot_loader_name {
    struct mbi_tag header;
    uint8_t string[0];
} __attribute__((packed));

struct mbi_tag_modules {
    struct mbi_tag header;
    uint32_t mod_start;
    uint32_t mod_end;
    uint8_t string[0];
} __attribute__((packed));

struct mbi_tag_basic_memory_information {
    struct mbi_tag header;
    uint32_t mem_lower;
    uint32_t mem_upper;
} __attribute__((packed));

struct mbi_tag_memory_map_entry {
    uint64_t base_addr;
    uint64_t length;
#define MBI_TAG_MEMORY_MAP_ENTRY_TYPE_AVAILABLE 1
#define MBI_TAG_MEMORY_MAP_ENTRY_TYPE_ACPI 3
#define MBI_TAG_MEMORY_MAP_ENTRY_TYPE_RESERVED 4
#define MBI_TAG_MEMORY_MAP_ENTRY_TYPE_DEFECTIVE 5
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed));

struct mbi_tag_memory_map {
    struct mbi_tag header;
#define MBI_TAG_MEMORY_MAP_ENTRY_SIZE 24
    uint32_t entry_size;
#define MBI_TAG_MEMORY_MAP_ENTRY_VERSION 0
    uint32_t entry_version;
    struct mbi_tag_memory_map_entry entries[0];
} __attribute__((packed));

struct mbi_tag_framebuffer_info_color_info {
    uint8_t framebuffer_red_field_position;
    uint8_t framebuffer_red_mask_size;
    uint8_t framebuffer_green_field_position;
    uint8_t framebuffer_green_mask_size;
    uint8_t framebuffer_blue_field_position;
    uint8_t framebuffer_blue_mask_size;
} __attribute__((packed));

struct mbi_tag_framebuffer_info {
    struct mbi_tag header;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
#define MBI_TAG_FRAMEBUFFER_INFO_FRAMEBUFFER_TYPE 1
    uint8_t framebuffer_type;
    uint16_t reserved;
    struct mbi_tag_framebuffer_info_color_info color_info;
} __attribute__((packed));

struct mbi_tag_elf_symbols {
    struct mbi_tag header;
    uint16_t num;
    uint16_t entsize;
    uint16_t shndx;
    uint16_t reserved;
} __attribute__((packed));

struct mbi_tag_efi_32_bit_system_table_pointer {
    struct mbi_tag header;
    uint32_t pointer;
} __attribute__((packed));

struct mbi_tag_efi_64_bit_system_table_pointer {
    struct mbi_tag header;
    uint64_t pointer;
} __attribute__((packed));

struct mbi_tag_smbios_tables {
    struct mbi_tag header;
    uint8_t major;
    uint8_t minor;
    uint8_t reserved[6];
} __attribute__((packed));

struct mbi_tag_acpi_old_rsdp {
    struct mbi_tag header;
    struct acpi_old_rsdp rsdp;
} __attribute__((packed));

struct mbi_tag_acpi_new_rsdp {
    struct mbi_tag header;
    struct acpi_new_rsdp rsdp;
} __attribute__((packed));

struct mbi_tag_efi_memory_map {
    struct mbi_tag header;
    uint32_t descriptor_size;
    uint32_t descriptor_version;
} __attribute__((packed));

struct mbi_tag_efi_32_bit_image_handle_pointer {
    struct mbi_tag header;
    uint32_t pointer;
} __attribute__((packed));

struct mbi_tag_efi_64_bit_image_handle_pointer {
    struct mbi_tag header;
    uint64_t pointer;
} __attribute__((packed));

struct mbi_tag_image_load_base_physical_address {
    struct mbi_tag header;
    uint32_t load_base_addr;
} __attribute__((packed));

#endif/* !MULTIBOOT2_MBI_H*/
