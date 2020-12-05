# This file's working directory is the root of the project

include Make.defaults

PRIMARY		:= kernel
GIVENS		:= 	$(SRC_DIR)/$(BOOTLOADER)/bootloader/bootloader.h $(SRC_DIR)/$(LIBRARY)/library.h $(THIS)/kernel.h $(SRC_DIR)/universal.h

SUBDIRS		:= $(shell find $(THIS)/* -maxdepth 1 -type d)
TARGETS		:= $(OBJ_DIR)/$(PRIMARY).elf
INCLUDES	:= $(addprefix -include , $(GIVENS))
LIBINCLUDES	:= $(addprefix -I, $(addsuffix /, $(shell find $(SRC_DIR)/$(LIBRARY)/* -maxdepth 1 -type d)))

export CFLAGS	:= $(INCLUDES) $(LIBINCLUDES) -I$(THIS)/
LDFLAGS			:= -T $(THIS)/kernel.ld -L$(LIB_DIR) -static -Bsymbolic -nostdlib
LDADDITIONS		= $(addprefix $(OBJ_DIR)/, $(filter-out $(PRIMARY).o, $(notdir $(wildcard $(OBJ_DIR)/*.o))))

ifdef ASMDUMP
	TARGETS	:= $(addprefix $(ASMDUMP_DIR)/, $(addsuffix .s, $(notdir $(basename $(TARGETS)))))
endif

all:	$(SUBDIRS) $(TARGETS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		$(MAKE) --no-print-directory -f $@/Makefile SUPER=$(THIS) THIS=$@

include Make.rules