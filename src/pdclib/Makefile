################################################################################
# This file is not native to PDClib and is a part of VisualOS
# Copyright 2020 Scott Maday
# Check the LICENSE file that came with this VisualOS for licensing terms
################################################################################
# This file's working directory is the root of the project

include Make.defaults

ifndef THIS_NODE
	THIS_NODE	:= $(THIS)
endif

ROOT_SUBDIRS	:= functions vos
SUBDIRS			:= $(shell find $(THIS_NODE)/* -maxdepth 0 -type d)

SOURCES			:= $(notdir $(wildcard $(THIS_NODE)/*.c $(THIS_NODE)/*.cpp $(THIS_NODE)/*.asm))
PRETARGETS		:= $(addprefix $(LIBC_DIR)/, $(addsuffix .o, $(subst .,_, $(SOURCES))))
TARGETS			:= $(LIBC_DIR)/$(LIBC).so $(LIBC_DIR)/$(LIBC).a
INCLUDE_DIRS	:= $(THIS)/include $(THIS)/vos/include

# TODO remove -D__STDC_NO_THREADS__
export CFLAGS	:= $(addprefix -I , $(INCLUDE_DIRS)) -fno-builtin -fvisibility=hidden -nostdlib -fno-builtin -g -fPIC -pedantic -D_STDC_PREDEF_H -D__STDC_NO_THREADS__ -pthread
CFLAGS			+= -Wall -Wextra -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Winline -Wno-long-long -Wuninitialized
CFLAGS			+= -Wmissing-prototypes -Wnested-externs -Wstrict-prototypes -Wdeclaration-after-statement
ifndef DEBUG_MODE
#	CFLAGS += -O1
endif
# export ASFLAGS	:= 

LDFLAGS	:= -shared -nostdlib
LDLIBS	= $(addprefix $(LIBC_DIR)/, $(notdir $(wildcard $(LIBC_DIR)/*.o)))
ARLIBS	= $(addprefix $(LIBC_DIR)/, $(notdir $(wildcard $(LIBC_DIR)/*.o)))

all:	setup
		+ $(MAKE) -f $(THIS)/Makefile $(addprefix $(THIS)/,$(ROOT_SUBDIRS)) pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	setup
setup:
		@mkdir -p $(LIBC_DIR)
		rm -f $(TARGETS)

.PHONY: $(SUBDIRS) subdirs
$(SUBDIRS):	
		+ $(MAKE) $(if $(wildcard $@/Makefile),-f $@/Makefile,-f $(THIS)/Makefile subdirs pretargets) THIS=$(THIS) THIS_NODE=$@
subdirs:	$(SUBDIRS)

.PHONY:	pretargets
pretargets:	$(PRETARGETS)


include Make.rules
