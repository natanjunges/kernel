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
#include <types.h>
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

DEFINE_VALUE_STATUS(uint32_t, uint32_t);

DEFINE_VALUE_STATUS(ioapic_id, struct ioapic_id);

DEFINE_VALUE_STATUS(ioapic_ver, struct ioapic_ver);

DEFINE_VALUE_STATUS(ioapic_arb, struct ioapic_arb);

DEFINE_VALUE_STATUS(ioapic_redirection_entry, union ioapic_redirection_entry);

DEFINE_VALUE_STATUS(acpi_madt_record_ioapic, const struct acpi_madt_record_ioapic *);

extern const struct acpi_madt_record_ioapic * ioapics[16];
extern const struct acpi_madt_record_iso * isos[16];

/**
 * Writes value to register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @param reg The register to be written to.
 * @param value The value to be written to the register.
 * @throws STATUS_NULL_POINTER_ARGUMENT self is null.
 */
STATUS ioapic_write(volatile uint32_t * const self, const uint32_t reg, const uint32_t value);

/**
 * Reads value from register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @param reg The register to be read from.
 * @return The value read from the register.
 * @throws STATUS_NULL_POINTER_ARGUMENT self is null.
 */
VALUE_STATUS(uint32_t) ioapic_read(volatile uint32_t * const self, const uint32_t reg);

/**
 * Reads value from ID register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @return The value read from the ID register.
 * @throws STATUS_NULL_POINTER_ARGUMENT self is null.
 */
VALUE_STATUS(ioapic_id) ioapic_read_id(volatile uint32_t * const self);

/**
 * Reads value from VER register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @return The value read from the VER register.
 * @throws STATUS_NULL_POINTER_ARGUMENT self is null.
 */
VALUE_STATUS(ioapic_ver) ioapic_read_ver(volatile uint32_t * const self);

/**
 * Reads value from ARB register of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @return The value read from the ARB register.
 * @throws STATUS_NULL_POINTER_ARGUMENT self is null.
 */
VALUE_STATUS(ioapic_arb) ioapic_read_arb(volatile uint32_t * const self);

/**
 * Writes redirection entry to REDTBL of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @param n The redirection entry number to be written.
 * @param entry The redirection entry to be written to the REDTBL.
 * @throws STATUS_NULL_POINTER_ARGUMENT self is null.
 */
STATUS ioapic_write_redirection_entry(volatile uint32_t * const self, const uint8_t n, const union ioapic_redirection_entry entry);

/**
 * Reads redirection entry from REDTBL of the IOAPIC with the provided base address.
 *
 * @param self The IOAPIC base address.
 * @param n The redirection entry number to be read.
 * @return The redirection entry read from the REDTBL.
 * @throws STATUS_NULL_POINTER_ARGUMENT self is null.
 */
VALUE_STATUS(ioapic_redirection_entry) ioapic_read_redirection_entry(volatile uint32_t * const self, const uint8_t n);

/**
 * Converts ISA IRQs to GSIs, and identity-maps the other ones.
 *
 * @param irq The IRQ to be converted.
 * @return The equivalent GSI.
 */
uint32_t ioapic_irq_to_gsi(const uint8_t irq);

/**
 * Finds the IOAPIC responsible for the provided GSI.
 *
 * @param gsi The provided GSI.
 * @return The IOAPIC responsible for the provided GSI.
 * @throws STATUS_NULL_POINTER_VALUE An IOAPIC record has a null base address.
 * @throws STATUS_NO_SUCH_THING No such IOAPIC was found.
 */
VALUE_STATUS(acpi_madt_record_ioapic) ioapic_gsi_to_ioapic(const uint32_t gsi);

/**
 * Gets the GSI from the provided IOAPIC and the redirection entry.
 *
 * @param ioapic The provided IOAPIC.
 * @param redirection_entry The provided redirection entry.
 * @return The equivalent GSI.
 * @throws STATUS_NULL_POINTER_ARGUMENT ioapic is null.
 * @throws STATUS_ARGUMENT_OUT_OF_BOUNDS redirection_entry is larger than max_redirection_entry.
 * @throws STATUS_NULL_POINTER_VALUE The IOAPIC record has a null base address.
 */
VALUE_STATUS(uint32_t) ioapic_redirection_entry_to_gsi(const struct acpi_madt_record_ioapic * const ioapic, const uint8_t redirection_entry);

/**
 * Goes through the MADT records and sets up the IOAPICs listed, masking all interrupts and setting up the ISA IRQs.
 *
 * @param madt The ACPI MADT table.
 * @param acpi_pid The ACPI processor ID of the main processor.
 * @throws STATUS_NULL_POINTER_ARGUMENT madt is null.
 * @throws STATUS_EMPTY_THING The MADT has no records.
 * @throws STATUS_TOO_MANY_THINGS The MADT records have more than 16 IOAPIC entries.
 * @throws STATUS_ILLEGAL_VALUE The ISO entries have non-ISA IRQs.
 * @throws STATUS_DUPLICATE_THING More than one ISO entry was found for the same IRQ.
 * @throws STATUS_NULL_POINTER_VALUE An IOAPIC record has a null base address.
 * @throws STATUS_NO_SUCH_THING The IOAPIC that handles some ISA IRQ was not found.
 */
STATUS ioapic_initialize(const struct acpi_madt * const madt, const uint8_t acpi_pid);
