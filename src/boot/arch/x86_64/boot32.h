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

#ifndef BOOT32_H
#define BOOT32_H

#define GDT_EXECUTABLE (1 << 43)
#define GDT_DESCRIPTOR_TYPE (1 << 44)
#define GDT_PRESENT (1 << 47)
#define GDT_64_BIT (1 << 53)

#define EFLAGS_ID (1 << 21)

#define CPUID_EXTENDED_FUNCTION 0x80000000
#define CPUID_EXTENDED_FEATURES 0x80000001
#define CPUID_INTEL_64 (1 << 29)

#define PAGING_PAGE_SIZE (1 << 7)
#define PAGING_READ_WRITE (1 << 1)
#define PAGING_PRESENT (1 << 0)

#define CR4_PAE (1 << 5)

#define MSR_IA32_EFER 0xC0000080
#define MSR_IA32_EFER_LME (1 << 8)

#define CR0_PG (1 << 31)

#endif/* !BOOT32_H*/
