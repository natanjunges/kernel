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

#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

struct acpi_old_rsdp {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct acpi_new_rsdp {
    struct acpi_old_rsdp old_rsdp;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct acpi_sdt {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct acpi_rsdt {
    struct acpi_sdt header;
    uint32_t sdt_pointers[0];
} __attribute__((packed));

struct acpi_xsdt {
    struct acpi_sdt header;
    uint64_t sdt_pointers[0];
} __attribute__((packed));

/**
 * Goes through SDTs in RSDT and finds the SDT whose signature matches the provided signature.
 *
 * @param self The RSDPv1.
 * @param signature The signature of the desired SDT.
 * @return The SDT whose signature matches the provided signature, or NULL if any of the arguments is NULL or no such SDT is found.
 */
struct acpi_sdt * acpi_old_rsdp_getSDT(struct acpi_old_rsdp * self, char signature[4]);

/**
 * Goes through SDTs in XSDT and finds the SDT whose signature matches the provided signature.
 *
 * @param self The RSDPv2+.
 * @param signature The signature of the desired SDT.
 * @return The SDT whose signature matches the provided signature, or NULL if any of the arguments is NULL or no such SDT is found.
 */
struct acpi_sdt * acpi_new_rsdp_getSDT(struct acpi_new_rsdp * self, char signature[4]);

#endif/* !ACPI_H*/
