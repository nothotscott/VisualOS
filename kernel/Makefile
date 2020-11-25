# This file's working directory is the root of the project

include Make.defaults

BOOTLOADER	:= gnu-efi/bootloader
GIVENS		:= 	$(BOOTLOADER)/bootloader.h $(THIS)/kernel.h

SUBDIRS		:= $(shell find $(THIS)/* -maxdepth 1 -type d)
INCLUDES	:= $(addprefix -include , $(GIVENS))

export CFLAGS	:= $(INCLUDES) -I$(THIS)/
LDFLAGS			:= -T $(THIS)/kernel.ld -static -Bsymbolic -nostdlib
LDADDITIONS		= $(addprefix $(OBJ_DIR)/, $(notdir $(wildcard $(OBJ_DIR)/*.o)))

all:	$(SUBDIRS) $(OBJ_DIR)/kernel.elf

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		$(MAKE) --no-print-directory -f $@/Makefile SUPER=$(THIS) THIS=$@

include Make.rules