# This file's working directory is the root of the project

include Make.defaults

BOOTLOADER	:= gnu-efi/bootloader
LIBRARY		:= library
GIVENS		:= 	$(BOOTLOADER)/bootloader.h $(LIBRARY)/library.h $(THIS)/kernel.h universal.h

SUBDIRS		:= $(shell find $(THIS)/* -maxdepth 1 -type d)
TARGETS		:= $(OBJ_DIR)/kernel.elf
INCLUDES	:= $(addprefix -include , $(GIVENS))
LIBINCLUDES	:= $(addprefix -I, $(addsuffix /, $(shell find $(LIBRARY)/* -maxdepth 1 -type d)))

export CFLAGS	:= $(INCLUDES) $(LIBINCLUDES) -I$(THIS)/
LDFLAGS			:= -T $(THIS)/kernel.ld -L$(LIB_DIR) -static -Bsymbolic -nostdlib
LDADDITIONS		= $(addprefix $(OBJ_DIR)/, $(notdir $(wildcard $(OBJ_DIR)/*.o)))

ifdef ASMDUMP
	TARGETS	:= $(addprefix $(ASMDUMP_DIR)/, $(addsuffix .s, $(notdir $(basename $(TARGETS)))))
endif

all:	$(SUBDIRS) $(TARGETS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		$(MAKE) --no-print-directory -f $@/Makefile SUPER=$(THIS) THIS=$@

include Make.rules