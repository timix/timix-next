ARCH_DIR=arch/nios2
NIOS_DIR?=/opt/altera/11.1sp1/nios2eds/

E2F := java -jar $(NIOS_DIR)/bin/elf2flash.jar

INCLUDES += -I$(ARCH_DIR)/include/
KERNEL_BOOT_HEAD = kernel/head.o
ARCH_OBJS += kernel/entry.o kernel/atomic.o kernel/io.o

###############################################################################
# Targets
###############################################################################
#
# SREC target
#
.PHONY: srec
srec: vmtimix.srec

#
# Builds an srec file suitable for being programmed.
#
# --input:  Input elf file of the kernel
# --output: SREC output.
# --base:   Flash memory base address.
# --end:    Flash memory end address.
# --reset:  Initial address.
#
SREC=vmtimix.srec
KVERYCLEANS += $(SREC)
$(SREC): $O/vmtimix
	@echo '    (SREC): ' $@
	$Q$(E2F) --input=$< --output=$@ --base=0 --end=0x8000 --reset=0
