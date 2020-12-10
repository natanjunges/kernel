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
#include <acpi/madt.h>

#define IOAPIC_REGSEL 0
#define IOAPIC_REGWIN 4

#define IOAPIC_ID 0
#define IOAPIC_VER 1
#define IOAPIC_ARB 2
#define IOAPIC_RED_ENTRY_LOWER(n) (0x10 + 2 * n)
#define IOAPIC_RED_ENTRY_HIGHER(n) (0x11 + 2 * n)

struct ioapic_id {
    const uint32_t reserved0:24;
    const uint32_t ioapic_id:4;
    const uint32_t reserved1:4;
};

struct ioapic_ver {
    const uint32_t version:9;
    const uint32_t reserved0:7;
    const uint32_t max_redirection_entry:8;
    const uint32_t reserved1:8;
};

struct ioapic_arb {
    const uint32_t reserved0:24;
    const uint32_t ioapic_arb_id:4;
    const uint32_t reserved1:4;
};

union ioapic_redirection_entry {
    struct {
        uint64_t vector:8;
#define IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_FIXED 0
#define IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_LOWEST 1
#define IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_SMI 2
#define IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_NMI 4
#define IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_INIT 5
#define IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_EXTINT 7
        uint64_t delivery_mode:3;
#define IOAPIC_REDIRECTION_ENTRY_DESTINATION_MODE_PHYSICAL 0
#define IOAPIC_REDIRECTION_ENTRY_DESTINATION_MODE_LOGICAL 1
        uint64_t destination_mode:1;
#define IOAPIC_REDIRECTION_ENTRY_DELIVERY_STATUS_RELAXED 0
#define IOAPIC_REDIRECTION_ENTRY_DELIVERY_STATUS_WAITING 1
        uint64_t delivery_status:1;
        uint64_t pin_polarity:1;
        uint64_t remote_irr:1;
        uint64_t trigger_mode:1;
#define IOAPIC_REDIRECTION_ENTRY_MASK_ENABLED 0
#define IOAPIC_REDIRECTION_ENTRY_MASK_DISABLED 1
        uint64_t mask:1;
        uint64_t reserved:39;
        uint64_t destination:8;
    };
    struct {
        uint32_t lower;
        uint32_t higher;
    };
};

extern const struct acpi_madt_record_ioapic * ioapics[256];
extern const struct acpi_madt_record_iso * isos[256];

/**
 * Writes value to register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @param reg The register to be written to.
 * @param value The value to be written to the register.
 */
void ioapic_write(volatile uint32_t * const self, const uint32_t reg, const uint32_t value);

/**
 * Reads value from register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @param reg The register to be read from.
 * @return The value read from the register.
 */
uint32_t ioapic_read(volatile uint32_t * const self, const uint32_t reg);

/**
 * Reads value from ID register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @return The value read from the ID register.
 */
struct ioapic_id ioapic_read_id(volatile uint32_t * const self);

/**
 * Reads value from VER register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @return The value read from the VER register.
 */
struct ioapic_ver ioapic_read_ver(volatile uint32_t * const self);

/**
 * Reads value from ARB register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @return The value read from the ARB register.
 */
struct ioapic_arb ioapic_read_arb(volatile uint32_t * const self);

/**
 * Writes redirection entry to REDTBL of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @param n The redirection entry number to be written.
 * @param entry The redirection entry to be written to the REDTBL.
 */
void ioapic_write_redirection_entry(volatile uint32_t * const self, const uint8_t n, const union ioapic_redirection_entry entry);

/**
 * Reads redirection entry from REDTBL of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @param n The redirection entry number to be read.
 * @return The redirection entry read from the REDTBL.
 */
union ioapic_redirection_entry ioapic_read_redirection_entry(volatile uint32_t * const self, const uint8_t n);

/**
 * Goes through the MADT records and sets up the IOAPICs listed, masking all interrupts and setting up the ISA IRQs.
 *
 * @param madt The ACPI MADT table.
 * @param acpi_pid The ACPI processor ID of the main processor.
 */
void ioapic_initialize(const struct acpi_madt * const madt, const uint8_t acpi_pid);
