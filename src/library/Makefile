# This file's working directory is the root of the project

include Make.defaults

GIVENS		:= $(THIS)/library.h $(SRC_DIR)/universal.h

LIBVOS		:= libvos
SUBDIRS		:= $(THIS)/$(LIBVOS) $(filter-out $(THIS)/$(LIBVOS), $(shell find $(THIS)/* -maxdepth 1 -type d))
INCLUDES	:= $(addprefix -include , $(GIVENS))

# TODO Stack protector after dynamic memory (-fstack-protector-all)
export CXXFLAGS		:= $(INCLUDES) -I$(THIS)/ -m64 -ffreestanding -mno-red-zone -fno-rtti -fno-exceptions -fPIC -O2

all:	$(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		$(MAKE) --no-print-directory -f $@/Makefile SUPER=$(THIS) THIS=$@ LIBVOS=$(LIBVOS)

include Make.rules