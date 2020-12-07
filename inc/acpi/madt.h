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

#ifndef MADT_H
#define MADT_H

#include <stdint.h>
#include <acpi/acpi.h>

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

struct acpi_madt_record * acpi_madt_nextRecord(struct acpi_madt * self, struct acpi_madt_record * record);

#endif/* !MADT_H*/
