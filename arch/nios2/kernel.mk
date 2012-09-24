ARCH_DIR=arch/nios2

INCLUDES += -I$(ARCH_DIR)/include/

KERNEL_BOOT_HEAD = kernel/head.o

ARCH_OBJS += kernel/entry.o kernel/atomic.o kernel/io.o
