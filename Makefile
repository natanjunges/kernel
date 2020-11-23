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
XLD ?= $(TARGET)-elf-ld
QEMU ?= qemu-system-$(TARGET)
PWD = $(shell pwd)
INC ?= $(PWD)/inc
BUILD ?= $(PWD)/build
SUBMAKES = boot

linker_ld := $(PWD)/src/linker.ld
grub_cfg := $(PWD)/src/boot/grub.cfg
binary := $(BUILD)/kernel-$(TARGET).elf
iso := $(BUILD)/kernel-$(TARGET).iso

.PHONY: all iso run clean kernel $(SUBMAKES)

all: kernel

iso: kernel $(iso)

run: kernel $(iso)
	$(QEMU) -bios OVMF.fd -cdrom $(iso)

clean:
	@rm -r $(BUILD)

kernel: $(SUBMAKES)
	make $(binary)

$(SUBMAKES):
	make -C $(PWD)/src/$@ all TARGET=$(TARGET) XCC=$(XCC) XLD=$(XLD) QEMU=$(QEMU) INC=$(INC) BUILD=$(BUILD)

$(binary): $(linker_ld) $(shell find $(BUILD) -name *.o)
	$(XLD) -n -T $(linker_ld) $(shell find $(BUILD) -name *.o) -o $@

$(iso): $(binary) $(grub_cfg)
	mkdir -p $(BUILD)/isofiles/boot/grub/
	cp $(binary) $(BUILD)/isofiles/boot/kernel.elf
	cp $(grub_cfg) $(BUILD)/isofiles/boot/grub/
	grub-mkrescue -o $@ $(BUILD)/isofiles/ 2> /dev/null
	rm -r $(BUILD)/isofiles/
