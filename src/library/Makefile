# This file's working directory is the root of the project

include Make.defaults

GIVENS		:= $(THIS)/library.h $(SRC_DIR)/universal.h
LIBVOS		:= libvos
SUBDIRS		:= $(shell find $(THIS)/* -maxdepth 1 -type d)
INCLUDES	:= $(addprefix -include , $(GIVENS))


# TODO Stack protector after dynamic memory (-fstack-protector-all)
export CXXFLAGS		:= $(INCLUDES) -I$(THIS)/ -m64 -ffreestanding -mno-red-zone -fno-rtti -fno-exceptions -fPIC -g
ifndef DEBUG_MODE
	CXXFLAGS += -O2
endif
export ASFLAGS		:=  -f elf64 -F dwarf -g

LIBNAME	:= $(LIBVOS)
include $(THIS)/Make.generic

all:	setup
		$(MAKE) -f $(THIS)/Makefile $(TARGETS)
		$(MAKE) -f $(THIS)/Makefile $(SUBDIRS)

.PHONY:	setup
setup:
		@mkdir -p $(LIB_DIR)

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		+ $(MAKE) -f $@/Makefile SUPER=$(THIS) SUPER=$(THIS) THIS=$@ LIBVOS=$(LIBVOS)

include Make.rules