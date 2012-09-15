ARCH_DIR=arch/arm

CFLAGS += -mcpu=cortex-a9 -mfloat-abi=softfp -mabi=aapcs-linux -mno-thumb-interwork
LDFLAGS += -T $(ARCH_DIR)/kernel.ld

KERNEL_BOOT_HEAD = head.o
ARCH_OBJS += entry.o
