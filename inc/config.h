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

#include <multiboot2.h>

#define FRAMEBUFFER 1

#if FRAMEBUFFER
#define FRAMEBUFFER_WIDTH 0
#define FRAMEBUFFER_HEIGHT 0
#define FRAMEBUFFER_DEPTH 0
#endif/* FRAMEBUFFER*/

#define MODULE_ALIGNMENT 0

#define EFI_BOOT_SERVICES 0

#define RELOCATABLE_HEADER 0

#if RELOCATABLE_HEADER
#define RELOCATABLE_HEADER_MIN_ADDR 0x00100000
#define RELOCATABLE_HEADER_MAX_ADDR 0xFFFFFFFF
#define RELOCATABLE_HEADER_ALIGN 4096
#define RELOCATABLE_HEADER_PREFERENCE MULTIBOOT2_HEADER_TAG_RELOCATABLE_HEADER_PREFERENCE_NONE
#endif/* RELOCATABLE_HEADER*/

#define STACK_SIZE 0x4000
