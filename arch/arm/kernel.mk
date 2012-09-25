ARCH_DIR=arch/arm

INCLUDES += -I$(ARCH_DIR)/include/
CFLAGS += -mcpu=cortex-a9 -mfloat-abi=softfp -mabi=aapcs-linux -mno-thumb-interwork
LDFLAGS += -T $(ARCH_DIR)/kernel.ld

KERNEL_BOOT_HEAD = kernel/head.o
ARCH_OBJS += kernel/entry.o kernel/io.o kernel/atomic.o
ARCH_OBJS += kernel/arm-arch.o

QEMU      ?= qemu-system-arm
QEMU_MACH ?= vexpress-a9
QEMU_MEM  ?= 16
QEMU_BOOT := $(QEMU) -m $(QEMU_MEM) -M $(QEMU_MACH) -kernel Image

KCLEANS += Image

all: Image

Image: $O/vmtimix
	@echo ' (OBJCOPY): ' $@
	$Q $(OBJCOPY) -O binary $< $@

#
# Run on qemu
#
boot: Image
	$(QEMU_BOOT)

#
# Run on qemu, wait for gdb to attach
#
gdb-boot: Image
	$(QEMU_BOOT) -S -s
