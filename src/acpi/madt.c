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
#include <acpi/madt.h>

VALUE_STATUS(acpi_madt_record) acpi_madt_next_record(const struct acpi_madt * const self, const struct acpi_madt_record * const record) {
    if (self == null || record == null) {
        return (VALUE_STATUS(acpi_madt_record)){.status = STATUS_NULL_POINTER_ARGUMENT};
    } else if (record < self->records || (uintptr_t)record >= (uintptr_t)self + self->header.length) {
        return (VALUE_STATUS(acpi_madt_record)){.status = STATUS_ARGUMENT_OUT_OF_BOUNDS};
    }

    const struct acpi_madt_record * const ret = (const struct acpi_madt_record *)((uintptr_t)record + record->length);

    if (ret < self->records || (uintptr_t)ret >= (uintptr_t)self + self->header.length) {
        return (VALUE_STATUS(acpi_madt_record)){.status = STATUS_VALUE_OUT_OF_BOUNDS};
    }

    return (VALUE_STATUS(acpi_madt_record)){.value = ret, .status = STATUS_OK};
}
