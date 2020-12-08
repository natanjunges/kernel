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

#pragma once

#include <stdint.h>
#include <multiboot2.h>
#include <acpi/acpi.h>

struct mbi_tag {
    const uint32_t type;
    const uint32_t size;
} __attribute__((packed));

struct mbi_header {
    const uint32_t total_size;
    const uint32_t reserved;
    const struct mbi_tag tags[0];
} __attribute__((packed));

struct mbi_tag_boot_command_line {
    const struct mbi_tag header;
    const char string[0];
} __attribute__((packed));

struct mbi_tag_boot_loader_name {
    const struct mbi_tag header;
    const char string[0];
} __attribute__((packed));

struct mbi_tag_modules {
    const struct mbi_tag header;
    const uint32_t mod_start;
    const uint32_t mod_end;
    const char string[0];
} __attribute__((packed));

struct mbi_tag_basic_memory_information {
    const struct mbi_tag header;
    const uint32_t mem_lower;
    const uint32_t mem_upper;
} __attribute__((packed));

struct mbi_tag_memory_map_entry {
    const uint64_t base_addr;
    const uint64_t length;
#define MBI_TAG_MEMORY_MAP_ENTRY_TYPE_AVAILABLE 1
#define MBI_TAG_MEMORY_MAP_ENTRY_TYPE_ACPI 3
#define MBI_TAG_MEMORY_MAP_ENTRY_TYPE_RESERVED 4
#define MBI_TAG_MEMORY_MAP_ENTRY_TYPE_DEFECTIVE 5
    const uint32_t type;
    const uint32_t reserved;
} __attribute__((packed));

struct mbi_tag_memory_map {
    const struct mbi_tag header;
#define MBI_TAG_MEMORY_MAP_ENTRY_SIZE 24
    const uint32_t entry_size;
#define MBI_TAG_MEMORY_MAP_ENTRY_VERSION 0
    const uint32_t entry_version;
    const struct mbi_tag_memory_map_entry entries[0];
} __attribute__((packed));

struct mbi_tag_framebuffer_info_color_info {
    const uint8_t framebuffer_red_field_position;
    const uint8_t framebuffer_red_mask_size;
    const uint8_t framebuffer_green_field_position;
    const uint8_t framebuffer_green_mask_size;
    const uint8_t framebuffer_blue_field_position;
    const uint8_t framebuffer_blue_mask_size;
} __attribute__((packed));

struct mbi_tag_framebuffer_info {
    const struct mbi_tag header;
    const uint64_t framebuffer_addr;
    const uint32_t framebuffer_pitch;
    const uint32_t framebuffer_width;
    const uint32_t framebuffer_height;
    const uint8_t framebuffer_bpp;
#define MBI_TAG_FRAMEBUFFER_INFO_FRAMEBUFFER_TYPE 1
    const uint8_t framebuffer_type;
    const uint16_t reserved;
    const struct mbi_tag_framebuffer_info_color_info color_info;
} __attribute__((packed));

struct mbi_tag_elf_symbols {
    const struct mbi_tag header;
    const uint16_t num;
    const uint16_t entsize;
    const uint16_t shndx;
    const uint16_t reserved;
} __attribute__((packed));

struct mbi_tag_efi_32_bit_system_table_pointer {
    const struct mbi_tag header;
    const uint32_t pointer;
} __attribute__((packed));

struct mbi_tag_efi_64_bit_system_table_pointer {
    const struct mbi_tag header;
    const uint64_t pointer;
} __attribute__((packed));

struct mbi_tag_smbios_tables {
    const struct mbi_tag header;
    const uint8_t major;
    const uint8_t minor;
    const uint8_t reserved[6];
} __attribute__((packed));

struct mbi_tag_acpi_old_rsdp {
    const struct mbi_tag header;
    const struct acpi_old_rsdp rsdp;
} __attribute__((packed));

struct mbi_tag_acpi_new_rsdp {
    const struct mbi_tag header;
    const struct acpi_new_rsdp rsdp;
} __attribute__((packed));

struct mbi_tag_efi_memory_map {
    const struct mbi_tag header;
    const uint32_t descriptor_size;
    const uint32_t descriptor_version;
} __attribute__((packed));

struct mbi_tag_efi_32_bit_image_handle_pointer {
    const struct mbi_tag header;
    const uint32_t pointer;
} __attribute__((packed));

struct mbi_tag_efi_64_bit_image_handle_pointer {
    const struct mbi_tag header;
    const uint64_t pointer;
} __attribute__((packed));

struct mbi_tag_image_load_base_physical_address {
    const struct mbi_tag header;
    const uint32_t load_base_addr;
} __attribute__((packed));
