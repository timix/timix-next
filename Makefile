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

############## VARIABLE DEFINITIONS #################
# TODO: Arch assigned to the host arch.
#####################################################
ARCH        ?= x86
KDIR        = kernel
INCDIR      = include
BUILDDIR    = build
DEP_SUFFIX  = d

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
# Drivers Objects
-include $(KDIR)/kernel.mk
# Tools
-include $(TOOLSDIR)/tools.mk

###################### Full Targets ############################
KERNEL_OBJS += $(DRIVER_OBJS:%=drivers/%)
KERNEL_BOOT_HEAD := $(BUILDDIR)/arch/$(ARCH)/$(KERNEL_BOOT_HEAD)
KOBJS = $(KERNEL_OBJS:%=$(BUILDDIR)/$(KDIR)/%)
KOBJS += $(ARCH_OBJS:%=$(BUILDDIR)/arch/$(ARCH)/%)
ALL_KOBJS += $(KOBJS)
ALL_KOBJS += $(KERNEL_BOOT_HEAD)

###################### Build Rules ########################
$(BUILDDIR)/%.o: %.S
	@echo '      (AS): ' $@
	$Qmkdir -p `dirname $@`
	$Q$(CC) $(CFLAGS) -DASSEMBLER $(INCLUDES) -MD -MP -MT $(<:%.S=$(BUILDDIR)/%.o) -c -o $@ $<
	$Q$(OBJCOPY) -R .comment -R .note $@ $@

$(BUILDDIR)/%.o: %.c
	@echo '      (CC): ' $@
	$Qmkdir -p `dirname $@`
	$Q$(CC) $(CFLAGS) $(INCLUDES) -MD -MP -MT $(<:%.c=$(BUILDDIR)/%.o) -c -o $@ $<
	$Q$(OBJCOPY) -R .comment -R .note $@ $@

$(BUILDDIR)/%.a:
	@mkdir -p $(BUILDDIR)
	@echo '      (AR): ' $@
	@rm -f $@
	$Q$(AR) rc $@ $^

$(BUILDDIR)/vmtimix: $(KERNEL_BOOT_HEAD) $(KOBJS)
	@mkdir -p $(BUILDDIR)
	@echo '      (LD): ' $@
	$Q$(LD) -r $(LDFLAGS) -o $@.o $^
	$Q$(LD) $(LDFLAGS) -o $@ $@.o
	$Qrm -f $@.o

vmtimiz: $(BUILDDIR)/vmtimix
	@echo '    (GZIP): ' $@
	$Qgzip -c $< > $@

###################### DEPS ######################
KERN_DEPS = $(patsubst %.o, %.$(DEP_SUFFIX), $(ALL_KOBJS))
ifneq (,$(wildcard $(BUILDDIR)))
ifneq (,$(KERN_DEPS))
-include $(KERN_DEPS)
endif
endif

################# CLEANING RULES #####################
clean:
	@rm -f $(ALL_KOBJS)
	@rm -f $(KLIBS)
	@rm -f $(BUILDDIR)/vmtimix
	@rm -f $(KCLEANS)

veryclean: clean
	@rm -rf $(BUILDDIR)
	@rm -f vmtimiz
