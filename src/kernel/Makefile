# This file's working directory is the root of the project

include Make.defaults

PRIMARY		:= kernel
GIVENS		:= $(SRC_DIR)/$(LIBRARY)/library.h $(SRC_DIR)/universal.h $(THIS)/kernel.h

SOURCES		:= $(filter-out $(PRIMARY).c, $(notdir $(wildcard $(THIS)/*.c $(THIS)/*.cpp $(THIS)/*.asm)))
SUBDIRS		:= $(shell find $(THIS)/* -maxdepth 1 -type d)
PRETARGETS	:= $(addprefix $(KERNEL_DIR)/, $(addsuffix .o, $(basename $(SOURCES))))
TARGETS		:= $(KERNEL_DIR)/$(PRIMARY).elf
INCLUDES	:= $(addprefix -include , $(GIVENS))
LIBINCLUDES	:= $(addprefix -I, $(addsuffix /, $(shell find $(SRC_DIR)/$(LIBRARY)/* -maxdepth 1 -type d) $(SRC_DIR)/$(LIBRARY)))
LIBARCHIVES	:= $(addprefix $(LIB_DIR)/, $(notdir $(wildcard $(LIB_DIR)/*.a)))

export CFLAGS	:= $(INCLUDES) $(LIBINCLUDES) -I$(THIS)/ -m64 -ffreestanding -fshort-wchar -O1 -g
export ASFLAGS	:= -p $(THIS)/kernel.inc -i $(THIS)/ -f elf64 -F dwarf -g
LDFLAGS			:= -T $(THIS)/kernel.ld -L$(LIB_DIR) -static -Bsymbolic -nostdlib
LDLIBS			= $(addprefix $(KERNEL_DIR)/, $(filter-out $(PRIMARY).o, $(notdir $(wildcard $(KERNEL_DIR)/*.o)))) $(LIBARCHIVES)

ifdef ASMDUMP
	PRETARGETS	:= $(addprefix $(ASMDUMP_DIR)/, $(addsuffix .s, $(notdir $(basename $(PRETARGETS)))))
endif

# Parallelization forces us to make the real target recursivley
all:	$(SUBDIRS) $(PRETARGETS)
		$(MAKE) $(TARGETS) -f $(THIS)/Makefile


.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		+ $(MAKE) -f $@/Makefile SUPER=$(THIS) THIS=$@

include Make.rules