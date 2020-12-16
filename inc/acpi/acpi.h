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

struct acpi_old_rsdp {
    const char signature[8];
    const uint8_t checksum;
    const char oem_id[6];
    const uint8_t revision;
    const uint32_t rsdt_address;
} __attribute__((packed));

struct acpi_new_rsdp {
    const struct acpi_old_rsdp old_rsdp;
    const uint32_t length;
    const uint64_t xsdt_address;
    const uint8_t checksum;
    const uint8_t reserved[3];
} __attribute__((packed));

struct acpi_sdt {
    const char signature[4];
    const uint32_t length;
    const uint8_t revision;
    const uint8_t checksum;
    const char oem_id[6];
    const char oem_table_id[8];
    const uint32_t oem_revision;
    const uint32_t creator_id;
    const uint32_t creator_revision;
} __attribute__((packed));

struct acpi_rsdt {
    const struct acpi_sdt header;
    const uint32_t sdt_pointers[0];
} __attribute__((packed));

struct acpi_xsdt {
    const struct acpi_sdt header;
    const uint64_t sdt_pointers[0];
} __attribute__((packed));

DEFINE_VALUE_STATUS(acpi_sdt, const struct acpi_sdt *);

/**
 * Goes through SDTs in RSDT and finds the SDT whose signature matches the provided signature.
 *
 * @param self The RSDPv1.
 * @param signature The signature of the desired SDT.
 * @return The SDT whose signature matches the provided signature.
 * @throws STATUS_NULL_POINTER_ARGUMENT Any of the arguments is null.
 * @throws STATUS_NULL_POINTER_VALUE The pointer to the RSDT is null, or a pointer to one of its SDTs is.
 * @throws STATUS_NO_SUCH_THING No such SDT was found.
 */
VALUE_STATUS(acpi_sdt) acpi_old_rsdp_get_sdt(const struct acpi_old_rsdp * const self, const char signature[4]);

/**
 * Goes through SDTs in XSDT and finds the SDT whose signature matches the provided signature.
 *
 * @param self The RSDPv2+.
 * @param signature The signature of the desired SDT.
 * @return The SDT whose signature matches the provided signature.
 * @throws STATUS_NULL_POINTER_ARGUMENT Any of the arguments is null.
 * @throws STATUS_NULL_POINTER_VALUE The pointer to the XSDT is null, or a pointer to one of its SDTs is.
 * @throws STATUS_NO_SUCH_THING No such SDT was found.
 */
VALUE_STATUS(acpi_sdt) acpi_new_rsdp_get_sdt(const struct acpi_new_rsdp * const self, const char signature[4]);
