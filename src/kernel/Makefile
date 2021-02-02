# This file's working directory is the root of the project

include Make.defaults

PRIMARY		:= kernel
GIVENS		:= $(SRC_DIR)/$(LIBRARY)/library.h $(SRC_DIR)/universal.h $(THIS)/kernel.h

SOURCES		:= $(notdir $(wildcard $(THIS)/*.c $(THIS)/*.cpp $(THIS)/*.asm))
SUBDIRS		:= $(shell find $(THIS)/* -maxdepth 1 -type d)
PRETARGETS	:= $(addprefix $(KERNEL_DIR)/, $(addsuffix .o, $(basename $(SOURCES))))
TARGETS		:= $(KERNEL_DIR)/$(PRIMARY)_partial.o
INCLUDES	:= $(addprefix -include , $(GIVENS))
LIBINCLUDES	:= $(addprefix -I, $(addsuffix /, $(shell find $(SRC_DIR)/$(LIBRARY)/* -maxdepth 1 -type d) $(SRC_DIR)/$(LIBRARY)) $(SRC_DIR)/$(LIBC)/include $(LIBC_DIR)/obj/include)

export CFLAGS	:= $(INCLUDES) $(LIBINCLUDES) -I$(THIS)/ -m64 -ffreestanding -nostdinc -fshort-wchar -O1 -g
export ASFLAGS	:= -p $(THIS)/kernel.inc -i $(THIS)/ -f elf64 -F dwarf -g
LDFLAGS			:= -nostdlib
LDLIBS			= $(addprefix $(KERNEL_DIR)/, $(notdir $(wildcard $(KERNEL_DIR)/*.o)))

ifdef ASMDUMP
	PRETARGETS	:= $(addprefix $(ASMDUMP_DIR)/, $(addsuffix .s, $(notdir $(basename $(PRETARGETS)))))
endif

# Parallelization forces us to make the real target recursivley
all:	setup
		$(MAKE) -f $(THIS)/Makefile $(SUBDIRS)
		$(MAKE) -f $(THIS)/Makefile $(PRETARGETS)
		$(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	setup
setup:
		@mkdir -p $(KERNEL_DIR)


.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		+ $(MAKE) -f $@/Makefile SUPER=$(THIS) THIS=$@



include Make.rules