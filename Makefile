################Verbose Output######################
# Prefix commands that should be suppressed with $Q
####################################################
V ?= 0
ifneq (0,$(V))
Q :=
else
# Verbose disabled, silence output
Q := @
endif

###############Build Output Directory################
# Use O=. to build in-tree.
#####################################################
O ?= build

############## VARIABLE DEFINITIONS #################
# TODO: Arch assigned to the host arch.
#####################################################
ARCH        ?= x86
KDIR        = kernel
INCDIR      = include
LIBDIR      = lib

############# BUILD PARAMETER VARIABLES ################
CFLAGS = -nostdinc -fno-strict-aliasing -fno-builtin -Wall -Werror
CFLAGS += -ggdb
LDFLAGS = -static --fatal-warnings -nostdlib
INCLUDES += -I$(KDIR) -I$(INCDIR)

############## TOOLCHAIN DEFAULTS #################
CROSS_COMPILE     ?=
CC                = $(CROSS_COMPILE)gcc
LD                = $(CROSS_COMPILE)ld
AR                = $(CROSS_COMPILE)ar
OBJCOPY           = $(CROSS_COMPILE)objcopy

.PHONY: all clean veryclean
all: vmtimiz

KCLEANS :=
KVERYCLEANS :=

# Platform makefile
-include arch/$(ARCH)/kernel.mk
# General Kernel Objects
-include $(KDIR)/kernel.mk
# Library Objects
-include $(LIBDIR)/kernel.mk

###################### Full Targets ############################
KERNEL_OBJS += $(DRIVER_OBJS:%=drivers/%)
KERNEL_BOOT_HEAD := $O/arch/$(ARCH)/$(KERNEL_BOOT_HEAD)
KOBJS += $(KERNEL_OBJS:%=$O/$(KDIR)/%)
KOBJS += $(ARCH_OBJS:%=$O/arch/$(ARCH)/%)
KOBJS += $(LIB_OBJS:%=$O/lib/%)
ALL_KOBJS += $(KOBJS)
ALL_KOBJS += $(KERNEL_BOOT_HEAD)

###################### Build Rules ########################
$O/%.o: %.S
	@echo '      (AS): ' $@
	$Qmkdir -p `dirname $@`
	$Q$(CC) $(CFLAGS) -DASSEMBLER $(INCLUDES) -MD -MP -MT $(<:%.S=$O/%.o) -c -o $@ $<
	$Q$(OBJCOPY) -R .comment -R .note $@ $@

$O/%.o: %.c
	@echo '      (CC): ' $@
	$Qmkdir -p `dirname $@`
	$Q$(CC) $(CFLAGS) $(INCLUDES) -MD -MP -MT $(<:%.c=$O/%.o) -c -o $@ $<
	$Q$(OBJCOPY) -R .comment -R .note $@ $@

$O/%.a:
	@mkdir -p $O
	@echo '      (AR): ' $@
	@rm -f $@
	$Q$(AR) rc $@ $^

$O/vmtimix: $(KERNEL_BOOT_HEAD) $(KOBJS)
	@mkdir -p $O
	@echo '      (LD): ' $@
	$Q$(LD) -r $(LDFLAGS) -o $@.o $^
	$Q$(LD) $(LDFLAGS) -o $@ $@.o
	$Qrm -f $@.o

vmtimiz: $O/vmtimix
	@echo '    (GZIP): ' $@
	$Qgzip -c $< > $@

###################### DEPS ######################
KERN_DEPS = $(patsubst %.o, %.d, $(ALL_KOBJS))
ifneq (,$(wildcard $O))
ifneq (,$(KERN_DEPS))
-include $(KERN_DEPS)
endif
endif

################# CLEANING RULES #####################
clean:
	@rm -f $(ALL_KOBJS)
	@rm -f $(KLIBS)
	@rm -f $O/vmtimix
	@rm -f $(KCLEANS)

veryclean: clean
	@rm -f vmtimiz
	@rm -f $(KVERYCLEANS)
