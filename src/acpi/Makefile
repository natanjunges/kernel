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

c_source_files := $(wildcard $(PWD)/*.c)
c_object_files := $(patsubst $(PWD)/%.c, \
	$(BUILD)/acpi/%.o, $(c_source_files))

.PHONY: all

all: $(c_object_files)

$(BUILD)/acpi/%.o: $(PWD)/%.c
	@mkdir -p $(shell dirname $@)
	$(XCC) -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Wall -Wextra -Werror -I$(INC) -c $< -o $@
