# This file's working directory is the root of the project

include Make.defaults

GIVENS		:= $(THIS)/library.h universal.h

SUBDIRS		:= $(shell find $(THIS)/* -maxdepth 1 -type d)
INCLUDES	:= $(addprefix -include , $(GIVENS))

# TODO Stack protector after dynamic memory (-fstack-protector-all)
export CXXFLAGS	:= $(INCLUDES) -I$(THIS)/ -ffreestanding -mno-red-zone -fno-rtti -fno-exceptions -fPIC

all:	$(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
		$(MAKE) --no-print-directory -f $@/Makefile SUPER=$(THIS) THIS=$@

include Make.rules