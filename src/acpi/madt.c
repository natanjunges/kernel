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
#include <acpi/madt.h>

const struct acpi_madt_record * acpi_madt_nextRecord(const struct acpi_madt * const self, const struct acpi_madt_record * const record) {
    if (self == 0 || record == 0 || record < self->records || (uintptr_t)record >= (uintptr_t)self + self->header.length) {
        return 0;
    }

    const struct acpi_madt_record * ret = record;
    ret = (const struct acpi_madt_record *)((uintptr_t)ret + ret->length);

    if ((uintptr_t)ret >= (uintptr_t)self + self->header.length) {
        return 0;
    }

    return ret;
}
