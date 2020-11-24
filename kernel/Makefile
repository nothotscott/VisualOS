# This file's working directory is the root of the project

include Make.defaults

BOOTLOADER	:= gnu-efi/bootloader
SUBDIRS		:= $(shell find $(THIS)/* -maxdepth 1 -type d)

export CFLAGS	:= -I$(BOOTLOADER) -I$(THIS)
LDFLAGS			:= -T $(THIS)/kernel.ld -static -Bsymbolic -nostdlib
LDADDITIONS		= $(addprefix $(OBJ_DIR)/, $(notdir $(wildcard $(OBJ_DIR)/*.o)))

all:	$(SUBDIRS) $(OBJ_DIR)/kernel.elf

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		$(MAKE) -f $@/Makefile THIS=$@

include Make.rules