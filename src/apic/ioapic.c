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

#include <stdint.h>
#include <acpi/acpi.h>
#include <acpi/madt.h>
#include <apic/ioapic.h>

void ioapic_write(volatile uint32_t * const self, const uint32_t reg, const uint32_t value) {
    if (self == 0) {
        return;
    }

    self[IOAPIC_REGSEL] = reg;
    self[IOAPIC_REGWIN] = value;
}

uint32_t ioapic_read(volatile uint32_t * const self, const uint32_t reg) {
    if (self == 0) {
        return 0;
    }

    self[IOAPIC_REGSEL] = reg;
    return self[IOAPIC_REGWIN];
}

struct ioapic_id ioapic_read_id(volatile uint32_t * const self) {
    struct ioapic_id id = {0};

    if (self != 0) {
        *(uint32_t *)(&id) = ioapic_read(self, IOAPIC_ID);
    }

    return id;
}

struct ioapic_ver ioapic_read_ver(volatile uint32_t * const self) {
    struct ioapic_ver ver = {0};

    if (self != 0) {
        *(uint32_t *)(&ver) = ioapic_read(self, IOAPIC_VER);
    }

    return ver;
}

struct ioapic_arb ioapic_read_arb(volatile uint32_t * const self) {
    struct ioapic_arb arb = {0};

    if (self != 0) {
        *(uint32_t *)(&arb) = ioapic_read(self, IOAPIC_ARB);
    }

    return arb;
}

void ioapic_write_redirection_entry(volatile uint32_t * const self, const uint8_t n, const union ioapic_redirection_entry entry) {
    if (self != 0) {
        ioapic_write(self, IOAPIC_RED_ENTRY_LOWER(n), entry.lower);
        ioapic_write(self, IOAPIC_RED_ENTRY_HIGHER(n), entry.higher);
    }
}

union ioapic_redirection_entry ioapic_read_redirection_entry(volatile uint32_t * const self, const uint8_t n) {
    union ioapic_redirection_entry entry = {0};

    if (self != 0) {
        entry.lower = ioapic_read(self, IOAPIC_RED_ENTRY_LOWER(n));
        entry.higher = ioapic_read(self, IOAPIC_RED_ENTRY_HIGHER(n));
    }

    return entry;
}

void initialize_ioapics(const struct acpi_madt * const madt) {
    if (madt == 0) {
        return;
    }

    const struct acpi_madt_record_ioapic * ioapics[256] = {0};
    const struct acpi_madt_record_iso * isos[256] = {0};
    const struct acpi_madt_record * record = madt->records;

    while (record != 0) {
        if (record->entry_type == ACPI_MADT_RECORD_ENTRY_TYPE_IOAPIC) {
            const struct acpi_madt_record_ioapic * const ioapic = (const struct acpi_madt_record_ioapic *)record;
            ioapics[ioapic->ioapic_id] = ioapic;
        } else if (record->entry_type == ACPI_MADT_RECORD_ENTRY_TYPE_ISO) {
            const struct acpi_madt_record_iso * const iso = (const struct acpi_madt_record_iso *)record;
            isos[iso->irq_source] = iso;
        }

        record = acpi_madt_next_record(madt, record);
    }

    for (unsigned int i = 0; i < 256; i++) {
        if (ioapics[i] != 0) {
            struct ioapic_ver ver = ioapic_read_ver((volatile uint32_t *)((uintptr_t)ioapics[i]->ioapic_address));
            uint32_t max_gsi = ioapics[i]->gsib + ver.max_redirection_entry;

            for (unsigned int j = 0; j <= ver.max_redirection_entry; j++) {
                union ioapic_redirection_entry entry = ioapic_read_redirection_entry((volatile uint32_t *)((uintptr_t)ioapics[i]->ioapic_address), j);
                entry.vector = 32 + ioapics[i]->gsib + j;
                entry.delivery_mode = IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_FIXED;
                entry.destination_mode = IOAPIC_REDIRECTION_ENTRY_DESTINATION_MODE_PHYSICAL;
                entry.pin_polarity = ACPI_MADT_RECORD_INTERRUPT_FLAGS_PIN_POLARITY_HIGH;
                entry.trigger_mode = ACPI_MADT_RECORD_INTERRUPT_FLAGS_TRIGGER_MODE_EDGE;
                entry.mask = IOAPIC_REDIRECTION_ENTRY_MASK_ENABLED;
                entry.destination = 0;
                ioapic_write_redirection_entry((volatile uint32_t *)((uintptr_t)ioapics[i]->ioapic_address), j, entry);
            }

            for (unsigned int j = 0; j < 256; j++) {
                if (isos[j] != 0 && isos[j]->gsi >= ioapics[i]->gsib && isos[j]->gsi <= max_gsi) {
                    union ioapic_redirection_entry entry = ioapic_read_redirection_entry((volatile uint32_t *)((uintptr_t)ioapics[i]->ioapic_address), isos[j]->gsi - ioapics[i]->gsib);
                    entry.pin_polarity = isos[j]->interrupt_flags.pin_polarity;
                    entry.trigger_mode = isos[j]->interrupt_flags.trigger_mode;
                    ioapic_write_redirection_entry((volatile uint32_t *)((uintptr_t)ioapics[i]->ioapic_address), isos[j]->gsi - ioapics[i]->gsib, entry);
                }
            }
        }
    }
}
