ARMGNU ?= aarch64-linux-gnu

COPS   = -Wall -ffreestanding -nostdlib -nostartfiles \
         -mgeneral-regs-only -mcpu=cortex-a72 -I. -Iinclude -Iinclude/drivers -Idrivers -Iirq -O2
ASMOPS = -I. -Iinclude -Iinclude/drivers -Idrivers -Iirq -mcpu=cortex-a72

BUILD_DIR = build

C_FILES   = $(wildcard kernel/*.c drivers/*.c irq/*.c mm/*.c lib/*.c game/*.c)
S_FILES   = $(wildcard boot/*.S irq/*.S)

OBJ_FILES = $(C_FILES:%.c=$(BUILD_DIR)/%_c.o) \
            $(S_FILES:%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:.o=.d)
-include $(DEP_FILES)

all: kernel8.img

$(BUILD_DIR)/%_c.o: %.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: %.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

kernel8.elf: $(OBJ_FILES) kernel.ld
	$(ARMGNU)-ld -T kernel.ld -o $@ $(OBJ_FILES)

kernel8.img: kernel8.elf
	$(ARMGNU)-objcopy -O binary $< $@

run:
	qemu-system-aarch64 -M raspi3b -cpu cortex-a72 \
	  -kernel kernel8.img -serial stdio -display none

debug:
	qemu-system-aarch64 -M raspi3b -cpu cortex-a72 \
	  -kernel kernel8.img -serial stdio -display none \
	  -s -S

clean:
	rm -rf $(BUILD_DIR) *.elf *.img
