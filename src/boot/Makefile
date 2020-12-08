# Copyright 2020 Natan Junges
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TARGET ?= x86_64
XCC ?= $(TARGET)-elf-gcc
PWD = $(shell pwd)
INC ?= $(PWD)/../../inc
BUILD ?= $(PWD)/../../build

assembly_source_files := $(wildcard $(PWD)/arch/$(TARGET)/*.S)
assembly_object_files := $(patsubst $(PWD)/arch/$(TARGET)/%.S, \
	$(BUILD)/boot/arch/$(TARGET)/%.o, $(assembly_source_files))
c_source_files := $(wildcard $(PWD)/*.c)
c_object_files := $(patsubst $(PWD)/%.c, \
	$(BUILD)/boot/%.o, $(c_source_files))

.PHONY: all

all: $(assembly_object_files) $(c_object_files)

$(BUILD)/boot/arch/$(TARGET)/%.o: $(PWD)/arch/$(TARGET)/%.S $(INC)/config.h
	@mkdir -p $(shell dirname $@)
	$(XCC) -ffreestanding -I$(INC) -c $< -o $@

$(BUILD)/boot/%.o: $(PWD)/%.c
	@mkdir -p $(shell dirname $@)
	$(XCC) -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -I$(INC) -c $< -o $@
