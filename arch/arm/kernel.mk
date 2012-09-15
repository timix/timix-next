ARCH_DIR=arch/arm

CFLAGS += -mcpu=cortex-a9 -mfloat-abi=softfp -mabi=aapcs-linux -mno-thumb-interwork
LDFLAGS += -T $(ARCH_DIR)/kernel.ld

KERNEL_BOOT_HEAD = head.o
ARCH_OBJS += entry.o

QEMU      ?= qemu-system-arm
QEMU_MACH ?= vexpress-a9
QEMU_MEM  ?= 16
QEMU_BOOT := $(QEMU) -m $(QEMU_MEM) -M $(QEMU_MACH) -kernel $(BUILDDIR)/vmtimix

#
# Run on qemu
#
boot: $(BUILDDIR)/vmtimix
	$(QEMU_BOOT)

#
# Run on qemu, wait for gdb to attach
#
gdb-boot: $(BUILDDIR)/vmtimix
	$(QEMU_BOOT) -S -s
