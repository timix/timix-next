
CFLAGS += -fno-omit-frame-pointer -fno-stack-protector -gdwarf-2 -m32
LDFLAGS += -melf_i386 -Ttext 100000

KERNEL_BOOT_HEAD = head.o
ARCH_OBJS += entry.o
