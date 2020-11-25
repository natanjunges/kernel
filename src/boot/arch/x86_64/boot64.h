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

#ifndef BOOT64_H
#define BOOT64_H

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

#define PIC_ICW1_INIT (1 << 4)
#define PIC_ICW1_ICW4 (1 << 0)

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

#define PIC2_MASK (1 << 2)
#define PIC2_ID (1 << 1)

#define PIC_ICW4_8086 (1 << 0)

#define PIC_MASK_ALL 0xFF

#define LAPIC_ENABLE (1 << 8)
#define LAPIC_SPURIOUS_INTERRUPT 0xFF

#define LAPIC_BASE 0xFEE00000
#define LAPIC_SIVR 0xF0

#define IDT_PRESENT (1 << 7)
#define IDT_STORAGE (1 << 4)
#define IDT_GATE_TYPE_INTERRUPT 0xE
#define IDT_GATE_TYPE_TRAP 0xF

#define PIC_EOI 0x20

#endif/* !BOOT64_H*/
