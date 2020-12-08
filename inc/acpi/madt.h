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
#include <acpi/acpi.h>

struct acpi_madt_record {
    const uint8_t entry_type;
    const uint8_t length;
} __attribute__((packed));

struct acpi_madt {
#define ACPI_SDT_SIGNATURE_MADT "APIC"
    const struct acpi_sdt header;
    const uint32_t lapic_address;
#define ACPI_MADT_FLAGS_MASK_PIC (1 << 0)
    const uint32_t flags;
    const struct acpi_madt_record records[0];
} __attribute__((packed));

struct acpi_madt_record_lapic {
#define ACPI_MADT_RECORD_ENTRY_TYPE_LAPIC 0
    const struct acpi_madt_record header;
    const uint8_t acpi_pid;
    const uint8_t apic_id;
#define ACPI_MADT_RECORD_LAPIC_FLAGS_ENABLED (1 << 0)
#define ACPI_MADT_RECORD_LAPIC_FLAGS_ONLINE (1 << 1)
    const uint32_t flags;
} __attribute__((packed));

struct acpi_madt_record_ioapic {
#define ACPI_MADT_RECORD_ENTRY_TYPE_IOAPIC 1
    const struct acpi_madt_record header;
    const uint8_t ioapic_id;
    const uint8_t reserved;
    const uint32_t ioapic_address;
    const uint32_t gsib;
} __attribute__((packed));

struct acpi_madt_record_iso {
#define ACPI_MADT_RECORD_ENTRY_TYPE_ISO 2
    const struct acpi_madt_record header;
    const uint8_t bus_source;
    const uint8_t irq_source;
    const uint32_t gsi;
    const uint16_t interrupt_flags;
} __attribute__((packed));

struct acpi_madt_record_nmi {
#define ACPI_MADT_RECORD_ENTRY_TYPE_NMI 4
    const struct acpi_madt_record header;
    const uint8_t acpi_pid;
    const uint16_t interrupt_flags;
#define ACPI_MADT_RECORD_NMI_LINT0 0
#define ACPI_MADT_RECORD_NMI_LINT1 1
    const uint8_t lint;
} __attribute__((packed));

struct acpi_madt_record_lapicao {
#define ACPI_MADT_RECORD_ENTRY_TYPE_LAPICAO 5
    const struct acpi_madt_record header;
    const uint16_t reserved;
    const uint64_t address;
} __attribute__((packed));

#define ACPI_MADT_RECORD_INTERRUPT_FLAGS_LOW (1 << 1)
#define ACPI_MADT_RECORD_INTERRUPT_FLAGS_LEVEL (1 << 3)

/**
 * Finds the next record in the MADT, provided the last accessed record.
 *
 * @param self The MADT.
 * @param record The last accessed record.
 * @return The next record in the MADT, or NULL if any of the arguments is NULL, record is not inside the MADT, or if it reaches the end of MADT.
 */
const struct acpi_madt_record * acpi_madt_next_record(const struct acpi_madt * const self, const struct acpi_madt_record * const record);
