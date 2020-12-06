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

#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

struct acpi_old_rsdp {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct acpi_new_rsdp {
    struct acpi_old_rsdp old_rsdp;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct acpi_sdt {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct acpi_rsdt {
    struct acpi_sdt header;
    uint32_t sdt_pointers[0];
} __attribute__((packed));

struct acpi_xsdt {
    struct acpi_sdt header;
    uint64_t sdt_pointers[0];
} __attribute__((packed));

struct acpi_madt_record {
    uint8_t entry_type;
    uint8_t length;
} __attribute__((packed));

struct acpi_madt {
#define ACPI_SDT_SIGNATURE_MADT "APIC"
    struct acpi_sdt header;
    uint32_t lapic_address;
#define ACPI_MADT_FLAGS_MASK_PIC (1 << 0)
    uint32_t flags;
    struct acpi_madt_record records[0];
} __attribute__((packed));

struct acpi_madt_record_lapic {
#define ACPI_MADT_RECORD_ENTRY_TYPE_LAPIC 0
    struct acpi_madt_record header;
    uint8_t acpi_pid;
    uint8_t apic_id;
#define ACPI_MADT_RECORD_LAPIC_FLAGS_ENABLED (1 << 0)
#define ACPI_MADT_RECORD_LAPIC_FLAGS_ONLINE (1 << 1)
    uint32_t flags;
} __attribute__((packed));

struct acpi_madt_record_ioapic {
#define ACPI_MADT_RECORD_ENTRY_TYPE_IOAPIC 1
    struct acpi_madt_record header;
    uint8_t ioapic_id;
    uint8_t reserved;
    uint32_t ioapic_address;
    uint32_t gsib;
} __attribute__((packed));

struct acpi_madt_record_iso {
#define ACPI_MADT_RECORD_ENTRY_TYPE_ISO 2
    struct acpi_madt_record header;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t gsi;
    uint16_t interrupt_flags;
} __attribute__((packed));

struct acpi_madt_record_nmi {
#define ACPI_MADT_RECORD_ENTRY_TYPE_NMI 4
    struct acpi_madt_record header;
    uint8_t acpi_pid;
    uint16_t interrupt_flags;
#define ACPI_MADT_RECORD_NMI_LINT0 0
#define ACPI_MADT_RECORD_NMI_LINT1 1
    uint8_t lint;
} __attribute__((packed));

struct acpi_madt_record_lapicao {
#define ACPI_MADT_RECORD_ENTRY_TYPE_LAPICAO 5
    struct acpi_madt_record header;
    uint16_t reserved;
    uint64_t address;
} __attribute__((packed));

#define ACPI_MADT_RECORD_INTERRUPT_FLAGS_LOW (1 << 1)
#define ACPI_MADT_RECORD_INTERRUPT_FLAGS_LEVEL (1 << 3)

#endif/* !ACPI_H*/
