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

struct acpi_sdt * acpi_old_rsdp_getSDT(struct acpi_old_rsdp * self, char signature[4]) {
    if (self == 0 || signature == 0) {
        return 0;
    }

    struct acpi_rsdt * rsdt = (struct acpi_rsdt *)((uintptr_t)self->rsdt_address);
    uint32_t length = (rsdt->header.length - sizeof(struct acpi_sdt))/4;

    for (uint32_t i = 0; i < length; i++) {
        struct acpi_sdt * sdt = (struct acpi_sdt *)((uintptr_t)rsdt->sdt_pointers[i]);

        if (sdt->signature[0] == signature[0] && sdt->signature[1] == signature[1] && sdt->signature[2] == signature[2] && sdt->signature[3] == signature[3]) {
            return sdt;
        }
    }

    return 0;
}

struct acpi_sdt * acpi_new_rsdp_getSDT(struct acpi_new_rsdp * self, char signature[4]) {
    if (self == 0 || signature == 0) {
        return 0;
    }

    struct acpi_xsdt * xsdt = (struct acpi_xsdt *)self->xsdt_address;
    uint32_t length = (xsdt->header.length - sizeof(struct acpi_sdt))/8;

    for (uint32_t i = 0; i < length; i++) {
        struct acpi_sdt * sdt = (struct acpi_sdt *)xsdt->sdt_pointers[i];

        if (sdt->signature[0] == signature[0] && sdt->signature[1] == signature[1] && sdt->signature[2] == signature[2] && sdt->signature[3] == signature[3]) {
            return sdt;
        }
    }

    return 0;
}
