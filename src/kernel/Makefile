# This file's working directory is the root of the project

include Make.defaults

ifndef THIS_NODE
	THIS_NODE	:= $(THIS)
endif

GIVENS		:= $(THIS)/kernel.h $(SRC_DIR)/$(LIBC)/include/stdint.h $(SRC_DIR)/$(LIBC)/include/stddef.h $(SRC_DIR)/$(LIBC)/include/stdbool.h
SUBDIRS		:= $(shell find $(THIS_NODE)/* -maxdepth 0 -type d)

SOURCES				:= $(notdir $(wildcard $(THIS_NODE)/*.c $(THIS_NODE)/*.cpp $(THIS_NODE)/*.asm))
PRETARGETS			:= $(addprefix $(KERNEL_DIR)/, $(addsuffix .o, $(subst .,_, $(SOURCES))))
TARGETS				:= $(KERNEL_DIR)/$(KERNEL)_partial.o
INCLUDE_DIRS		:= $(THIS)/
INCLUDE_LIB_DIRS	:= $(SRC_DIR)/$(LIBC)/include $(SRC_DIR)/$(LIBC)/vos/include 

export CFLAGS	:= $(addprefix -include , $(GIVENS)) $(addprefix -I , $(INCLUDE_DIRS)) $(addprefix -isystem , $(INCLUDE_LIB_DIRS))
CFLAGS			+= -m64 -ffreestanding -nostdinc -fshort-wchar -gdwarf -fno-pic -fpie -fvar-tracking -std=c11 -mgeneral-regs-only -D__STDC_NO_THREADS__
ifdef DEBUG_MODE
	CFLAGS += -O0
else
	CFLAGS += -O1
endif
export ASFLAGS	:= -p $(THIS)/kernel.inc -i $(THIS)/ -i $(THIS_NODE)/ -f elf64 -F dwarf -g
LDFLAGS			:= -nostdlib
LDLIBS			= $(addprefix $(KERNEL_DIR)/, $(notdir $(wildcard $(KERNEL_DIR)/*.o)))

all:	setup
		+ $(MAKE) -f $(THIS)/Makefile subdirs pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)


.PHONY:	setup
setup:
		@mkdir -p $(KERNEL_DIR)
		rm -f $(TARGETS)

.PHONY: $(SUBDIRS) subdirs
$(SUBDIRS):	
		+ $(MAKE) -f $(THIS)/Makefile subdirs pretargets THIS=$(THIS) THIS_NODE=$@
subdirs:	$(SUBDIRS)

.PHONY:	pretargets
pretargets:	$(PRETARGETS)

include Make.rules