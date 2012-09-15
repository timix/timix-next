ARCH_DIR = arch/x86
BOOTFDDIR = $(ARCH_DIR)/bootfd

CFLAGS += -fno-omit-frame-pointer -fno-stack-protector -gdwarf-2 -m32
LDFLAGS += -melf_i386 -Ttext 100000
KCLEANS += bootfd.img

KERNEL_BOOT_HEAD = head.o
ARCH_OBJS += entry.o

QEMU ?= qemu-system-i386
QEMU_BOOT = $(QEMU) -fda bootfd.img

boot: bootfd.img
	$(QEMU_BOOT)

gdb-boot: bootfd.img
	$(QEMU_BOOT) -s -S

bootfd.img: vmtimiz
	@echo '  (GUNZIP): ' bootfd.img
	@gzip -cd $(BOOTFDDIR)/bootfd.img.gz > bootfd.img
	@echo '   (MCOPY): ' bootfd.img
	@mcopy -o -i "bootfd.img" vmtimiz \
													 $(BOOTFDDIR)/menu.lst \
													 ::/boot/
