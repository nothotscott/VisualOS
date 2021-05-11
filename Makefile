THIS		:= $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include		Defaults.mk

TARGET_DIR	= $(BUILD_DIR)

KERNEL_BIN		:= $(KERNEL_DIR)/kernel.elf
VOS_BIN			:= $(BUILD_DIR)/vos.elf

LDFLAGS	:= -T $(SRC_DIR)/vos.ld -static -pie --no-dynamic-linker --gc-sections -nostdlib
LDLIBS	= $(filter-out %$(USERSPACE_SUFFIX).a, $(shell find $(LIBS_DIR)/ -maxdepth 1 -type f -name '*.a') $(shell find $(LIBS_DIR)/ -maxdepth 1 -type f -name '*.ro'))

###############################################################################

all:			$(BOOTLOADER) $(LIBC) $(KERNEL) $(BUILD_DIR)/$(OSNAME).img

.PHONY: setup
setup:	
				@mkdir -p $(BUILD_DIR)
ifdef ASMDUMP
				@mkdir -p $(ASMDUMP_DIR)
endif

###############################################################################

.PHONY:	libs
libs:			setup
				$(call echo_build)
				+ $(MAKE) --no-print-directory -f $(SRC_DIR)/$(LIBS)/Makefile THIS=$(SRC_DIR)/$(LIBS) TARGET_DIR=$(LIBS_DIR)

.PHONY: kernel $(KERNEL_DIR)/kernel.ro
kernel:			setup
				$(call echo_build)
				+ $(MAKE) --no-print-directory -f $(SRC_DIR)/$(KERNEL)/Makefile THIS=$(SRC_DIR)/$(KERNEL) TARGET_DIR=$(KERNEL_DIR)
$(KERNEL_DIR)/kernel.ro:	$(KERNEL)

###############################################################################

$(VOS_BIN):		$(KERNEL_BIN)
				mv $(KERNEL_BIN) $(VOS_BIN)
.PHONY: vos
vos:			$(VOS_BIN)

###############################################################################

RM_OBJ	:= rm -rf *.o *.a *.so *.elf
RM_IMG	:= rm -rf *.img *.iso *.vdi

.PHONY: clean-libs
clean-libs:			clean-img
					rm -rf $(LIBS_DIR)

.PHONY: clean-kernel
clean-kernel:		clean-img
					cd $(BUILD_DIR) && $(RM_OBJ)
					rm -rf $(KERNEL_DIR)/
ifneq "$(wildcard $(ASMDUMP_DIR))" ""
					cd $(ASMDUMP_DIR) && rm -rf *.s
endif

.PHONY: clean-img
clean-img:		
					cd $(BUILD_DIR) && $(RM_IMG)

.PHONY: clean
clean:		
					rm -rf $(BUILD_DIR)


include Rules.mk