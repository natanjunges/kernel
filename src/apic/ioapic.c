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
#include <types.h>
#include <acpi/acpi.h>
#include <acpi/madt.h>
#include <apic/ioapic.h>

const struct acpi_madt_record_ioapic * ioapics[16] = {0};
const struct acpi_madt_record_iso * isos[16] = {0};

STATUS ioapic_write(volatile uint32_t * const self, const uint32_t reg, const uint32_t value) {
    if (self == null) {
        return STATUS_NULL_POINTER_ARGUMENT;
    }

    self[IOAPIC_REGSEL] = reg;
    self[IOAPIC_REGWIN] = value;
    return STATUS_OK;
}

VALUE_STATUS(uint32_t) ioapic_read(volatile uint32_t * const self, const uint32_t reg) {
    if (self == null) {
        return (VALUE_STATUS(uint32_t)){.status = STATUS_NULL_POINTER_ARGUMENT};
    }

    self[IOAPIC_REGSEL] = reg;
    return (VALUE_STATUS(uint32_t)){.value = self[IOAPIC_REGWIN], .status = STATUS_OK};
}

VALUE_STATUS(ioapic_id) ioapic_read_id(volatile uint32_t * const self) {
    const VALUE_STATUS(uint32_t) id_vs = ioapic_read(self, IOAPIC_ID);

    if (id_vs.status != STATUS_OK) {
        return (VALUE_STATUS(ioapic_id)){.status = id_vs.status};
    }

    struct ioapic_id id;
    *(uint32_t *)(&id) = id_vs.value;
    return (VALUE_STATUS(ioapic_id)){.value = id, .status = STATUS_OK};
}

VALUE_STATUS(ioapic_ver) ioapic_read_ver(volatile uint32_t * const self) {
    const VALUE_STATUS(uint32_t) ver_vs = ioapic_read(self, IOAPIC_VER);

    if (ver_vs.status != STATUS_OK) {
        return (VALUE_STATUS(ioapic_ver)){.status = ver_vs.status};
    }

    struct ioapic_ver ver;
    *(uint32_t *)(&ver) = ver_vs.value;
    return (VALUE_STATUS(ioapic_ver)){.value = ver, .status = STATUS_OK};
}

VALUE_STATUS(ioapic_arb) ioapic_read_arb(volatile uint32_t * const self) {
    const VALUE_STATUS(uint32_t) arb_vs = ioapic_read(self, IOAPIC_ARB);

    if (arb_vs.status != STATUS_OK) {
        return (VALUE_STATUS(ioapic_arb)){.status = arb_vs.status};
    }

    struct ioapic_arb arb;
    *(uint32_t *)(&arb) = arb_vs.value;
    return (VALUE_STATUS(ioapic_arb)){.value = arb, .status = STATUS_OK};
}

STATUS ioapic_write_redirection_entry(volatile uint32_t * const self, const uint8_t n, const union ioapic_redirection_entry entry) {
    if (self == null) {
        return STATUS_NULL_POINTER_ARGUMENT;
    }

    ioapic_write(self, IOAPIC_RED_ENTRY_LOWER(n), entry.lower);
    ioapic_write(self, IOAPIC_RED_ENTRY_HIGHER(n), entry.higher);
    return STATUS_OK;
}

VALUE_STATUS(ioapic_redirection_entry) ioapic_read_redirection_entry(volatile uint32_t * const self, const uint8_t n) {
    if (self == null) {
        return (VALUE_STATUS(ioapic_redirection_entry)){.status = STATUS_NULL_POINTER_ARGUMENT};
    }

    union ioapic_redirection_entry entry;
    VALUE_STATUS(uint32_t) entry_vs = ioapic_read(self, IOAPIC_RED_ENTRY_LOWER(n));
    entry.lower = entry_vs.value;
    entry_vs = ioapic_read(self, IOAPIC_RED_ENTRY_HIGHER(n));
    entry.higher = entry_vs.value;
    return (VALUE_STATUS(ioapic_redirection_entry)){.value = entry, .status = STATUS_OK};
}

uint32_t ioapic_irq_to_gsi(const uint8_t irq) {
    if (irq < 16 && isos[irq] != null) {
        return isos[irq]->gsi;
    }

    return irq;
}

VALUE_STATUS(acpi_madt_record_ioapic) ioapic_gsi_to_ioapic(const uint32_t gsi) {
    for (unsigned int i = 0; i < 16 && ioapics[i] != null; i++) {
        const VALUE_STATUS(ioapic_ver) ver_vs = ioapic_read_ver((volatile uint32_t *)((uintptr_t)ioapics[i]->ioapic_address));

        if (ver_vs.status != STATUS_OK) {
            return (VALUE_STATUS(acpi_madt_record_ioapic)){.status = STATUS_NULL_POINTER_VALUE};
        }

        if (gsi >= ioapics[i]->gsib && gsi <= ioapics[i]->gsib + ver_vs.value.max_redirection_entry) {
            return (VALUE_STATUS(acpi_madt_record_ioapic)){.value = ioapics[i], .status = STATUS_OK};
        }
    }

    return (VALUE_STATUS(acpi_madt_record_ioapic)){.status = STATUS_NO_SUCH_THING};
}

VALUE_STATUS(uint32_t) ioapic_redirection_entry_to_gsi(const struct acpi_madt_record_ioapic * const ioapic, const uint8_t redirection_entry) {
    if (ioapic == null) {
        return (VALUE_STATUS(uint32_t)){.status = STATUS_NULL_POINTER_ARGUMENT};
    }

    const VALUE_STATUS(ioapic_ver) ver_vs = ioapic_read_ver((volatile uint32_t *)((uintptr_t)ioapic->ioapic_address));

    if (ver_vs.status != STATUS_OK) {
        return (VALUE_STATUS(uint32_t)){.status = STATUS_NULL_POINTER_VALUE};
    }

    if (redirection_entry > ver_vs.value.max_redirection_entry) {
        return (VALUE_STATUS(uint32_t)){.status = STATUS_ARGUMENT_OUT_OF_BOUNDS};
    }

    return (VALUE_STATUS(uint32_t)){.value = ioapic->gsib + redirection_entry, .status = STATUS_OK};
}

STATUS ioapic_initialize(const struct acpi_madt * const madt, const uint8_t acpi_pid) {
    if (madt == null) {
        return STATUS_NULL_POINTER_ARGUMENT;
    }

    if ((uintptr_t)madt->records >= (uintptr_t)madt + madt->header.length) {
        return STATUS_EMPTY_THING;
    }

    VALUE_STATUS(acpi_madt_record) record_vs = {.value = madt->records, .status = STATUS_OK};
    int n_ioapics = 0;

    while (record_vs.status != STATUS_VALUE_OUT_OF_BOUNDS) {
        if (record_vs.value->entry_type == ACPI_MADT_RECORD_ENTRY_TYPE_IOAPIC) {
            if (n_ioapics >= 16) {
                return STATUS_TOO_MANY_THINGS;
            }

            ioapics[n_ioapics] = (const struct acpi_madt_record_ioapic *)record_vs.value;
            n_ioapics++;
        } else if (record_vs.value->entry_type == ACPI_MADT_RECORD_ENTRY_TYPE_ISO) {
            const struct acpi_madt_record_iso * const iso = (const struct acpi_madt_record_iso *)record_vs.value;

            if (iso->bus_source != 0 || iso->irq_source >= 16) {
                return STATUS_ILLEGAL_VALUE;
            } else if (isos[iso->irq_source] != null) {
                return STATUS_DUPLICATE_THING;
            }

            isos[iso->irq_source] = iso;
        }

        record_vs = acpi_madt_next_record(madt, record_vs.value);
    }

    for (unsigned int i = 0; i < 16 && ioapics[i] != null; i++) {
        const VALUE_STATUS(ioapic_ver) ver_vs = ioapic_read_ver((volatile uint32_t *)((uintptr_t)ioapics[i]->ioapic_address));

        if (ver_vs.status != STATUS_OK) {
            return STATUS_NULL_POINTER_VALUE;
        }

        for (unsigned int j = 0; j <= ver_vs.value.max_redirection_entry; j++) {
            const union ioapic_redirection_entry entry = {.mask = IOAPIC_REDIRECTION_ENTRY_MASK_DISABLED};
            ioapic_write_redirection_entry((volatile uint32_t *)((uintptr_t)ioapics[i]->ioapic_address), j, entry);
        }
    }

    for (unsigned int i = 0; i < 16; i++) {
        if (i != 2) {
            union ioapic_redirection_entry entry = {
                .vector = 0,
                .delivery_mode = IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_FIXED,
                .destination_mode = IOAPIC_REDIRECTION_ENTRY_DESTINATION_MODE_PHYSICAL,
                .pin_polarity = ACPI_MADT_RECORD_INTERRUPT_FLAGS_PIN_POLARITY_HIGH,
                .trigger_mode = ACPI_MADT_RECORD_INTERRUPT_FLAGS_TRIGGER_MODE_EDGE,
                .mask = IOAPIC_REDIRECTION_ENTRY_MASK_DISABLED,
                .destination = acpi_pid
            };
            uint32_t gsi = ioapic_irq_to_gsi(i);
            VALUE_STATUS(acpi_madt_record_ioapic) ioapic_vs = ioapic_gsi_to_ioapic(gsi);

            if (ioapic_vs.status != STATUS_OK) {
                return ioapic_vs.status;
            }

            if (isos[i] != null) {
                if (isos[i]->interrupt_flags.pin_polarity_override) {
                    entry.pin_polarity = isos[i]->interrupt_flags.pin_polarity;
                }

                if (isos[i]->interrupt_flags.trigger_mode_override) {
                    entry.trigger_mode = isos[i]->interrupt_flags.trigger_mode;
                }
            }

            ioapic_write_redirection_entry((volatile uint32_t *)((uintptr_t)ioapic_vs.value->ioapic_address), gsi - ioapic_vs.value->gsib, entry);
        }
    }

    return STATUS_OK;
}
