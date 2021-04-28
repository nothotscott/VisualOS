# This file's working directory is the root of the project

include Make.defaults

GIVENS		:= $(THIS)/kernel.h $(SRC_DIR)/$(LIBS)/$(LIBC)/include/stdint.h $(SRC_DIR)/$(LIBS)/$(LIBC)/include/stddef.h $(SRC_DIR)/$(LIBS)/$(LIBC)/include/stdbool.h

SOURCES				:= $(call find_many,$(THIS),c cpp asm)
PRETARGETS			:= $(patsubst $(THIS)/%.o,$(KERNEL_DIR)/%.o,$(call to_obj,$(SOURCES)))
TARGETS				:= $(KERNEL_DIR)/$(KERNEL).ro
INCLUDE_DIRS		:= $(THIS)/
INCLUDE_LIB_DIRS	:= $(SRC_DIR)/$(LIBS)/$(LIBC)/include $(SRC_DIR)/$(LIBS)/$(LIBC)/vos/include 

export CFLAGS	:= $(addprefix -include , $(GIVENS)) $(addprefix -I ,$(INCLUDE_DIRS)) $(addprefix -isystem , $(INCLUDE_LIB_DIRS))
CFLAGS			+= -m64 -ffreestanding -nostdinc -fshort-wchar -gdwarf -fno-pic -fpie -fvar-tracking -std=c11 -mgeneral-regs-only -D__STDC_NO_THREADS__
ifdef DEBUG_MODE
	CFLAGS += -O0
else
	CFLAGS += -O1
endif
export ASFLAGS	:= -p $(THIS)/kernel.inc  $(addprefix -i ,$(INCLUDE_DIRS)) -f elf64 -F dwarf -g
LDFLAGS	:= -nostdlib
LDLIBS	= $(PRETARGETS)

all:	clean
		+ $(MAKE) -f $(THIS)/Makefile pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	pretargets
pretargets:	$(PRETARGETS)

.PHONY:		clean
ifneq "$(wildcard $(TARGETS))" ""
		rm -f $(TARGETS)
endif


include Make.rules