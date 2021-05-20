# This file's working directory is the root of the project

include Defaults.mk

GIVENS		:= $(THIS)/kernel.h $(SRC_DIR)/$(LIBS)/$(LIBC)/include/stdint.h $(SRC_DIR)/$(LIBS)/$(LIBC)/include/stddef.h $(SRC_DIR)/$(LIBS)/$(LIBC)/include/stdbool.h

SOURCES				:= $(call find_many,$(THIS),c cpp asm)
PRETARGETS			:= $(patsubst $(THIS)/%.o,$(KERNEL_DIR)/%.o,$(call to_obj,$(SOURCES)))
TARGETS				:= $(KERNEL_DIR)/$(KERNEL).ro

INCLUDE_DIRS		:= $(THIS)/
INCLUDE_LIB_DIRS	:= $(DEFAULT_LIBC_INCLUDES)

CFLAGS	:= $(addprefix -include , $(GIVENS)) $(addprefix -I ,$(INCLUDE_DIRS)) $(addprefix -isystem , $(INCLUDE_LIB_DIRS)) $(DEFAULT_CFLAGS) $(DEBUG_CFLAGS)
CFLAGS	+= -std=c11 -fshort-wchar 
ifeq ($(CC), gcc)
	CFLAGS	+= -mgeneral-regs-only
endif
ASFLAGS	:= -p $(THIS)/kernel.inc $(addprefix -i ,$(INCLUDE_DIRS)) $(DEFAULT_ASFLAGS) $(DEBUG_ASFLAGS)
LDFLAGS	:= -nostdlib
LDLIBS	= $(PRETARGETS)

all:	clean
		+ $(MAKE) -f $(THIS)/Makefile pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	pretargets
pretargets:	$(PRETARGETS)

.PHONY:		clean
clean:
ifneq "$(wildcard $(TARGETS))" ""
		rm -f $(TARGETS)
endif


include Rules.mk