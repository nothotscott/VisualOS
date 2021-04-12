################################################################################
# This file is not native to PDClib and is a part of VisualOS
# Copyright 2020 Scott Maday
# Check the LICENSE file that came with VisualOS for licensing terms
################################################################################
# This file's working directory is the root of the project

include Make.defaults

ifndef THIS_NODE
	THIS_NODE	:= $(THIS)
endif

VOS_PREFIX		:= _vos_
ROOT_SUBDIRS	:= functions vos
SUBDIRS			:= $(shell find $(THIS_NODE)/* -maxdepth 0 -type d)

SOURCES			:= $(notdir $(wildcard $(THIS_NODE)/*.c $(THIS_NODE)/*.cpp $(THIS_NODE)/*.asm))
PRETARGETS		:= $(addprefix $(LIBC_DIR)/, $(addsuffix .o, $(subst .,_, $(SOURCES))))
TARGETS			:= $(LIBC_DIR)/$(LIBC)$(KERNEL_SUFFIX).a $(LIBC_DIR)/$(LIBC)$(USERSPACE_SUFFIX).a
INCLUDE_DIRS	:= $(THIS)/include $(THIS)/vos/include

# TODO remove -D__STDC_NO_THREADS__ (might have to remove -pedantic for syscalls with 0 args)
export CFLAGS	:= $(addprefix -I , $(INCLUDE_DIRS)) -fno-builtin -fvisibility=hidden -fno-builtin -fshort-wchar -std=c11 -nostdlib -gdwarf -O0 -fno-pic -fpie -pedantic -D_STDC_PREDEF_H -D__STDC_NO_THREADS__ -pthread
CFLAGS			+= -Wall -Wextra -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Winline -Wno-long-long -Wuninitialized
CFLAGS			+= -Wnested-externs -Wdeclaration-after-statement
ifndef DEBUG_MODE
#	CFLAGS += -O1
endif
# export ASFLAGS	:= 

# LDFLAGS	:= -shared -nostdlib
# LDLIBS	= $(addprefix $(LIBC_DIR)/, $(notdir $(wildcard $(LIBC_DIR)/*.o)))
ARLIBS	= $(addprefix $(LIBC_DIR)/, $(notdir $(wildcard $(LIBC_DIR)/*.o)))

all:	setup
		+ $(MAKE) -f $(THIS)/Makefile $(addprefix $(THIS)/,$(ROOT_SUBDIRS)) pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	setup
setup:
		@mkdir -p $(LIBC_DIR)
		rm -f $(TARGETS)

.PHONY: 	$(SUBDIRS) subdirs
$(SUBDIRS):	
		+ $(MAKE) $(if $(wildcard $@/Makefile),\
			-f $@/Makefile SUPER=$(THIS) THIS=$@ TARGET_DIR=$(TARGET_DIR)/$(notdir $@),\
			-f $(THIS)/Makefile subdirs pretargets THIS=$(THIS)\
		) THIS_NODE=$@
subdirs:	$(SUBDIRS)
			@:

.PHONY:		pretargets
pretargets:	$(PRETARGETS)
			@:


$(TARGET_DIR)/%$(KERNEL_SUFFIX).a:
			$(AR) $(ARFLAGS) $@ $(filter-out $(addprefix %$(USERSPACE_SUFFIX), _c.o _asm.o _cpp.o), $(ARLIBS))

$(TARGET_DIR)/%$(USERSPACE_SUFFIX).a:
			$(AR) $(ARFLAGS) $@ $(filter-out $(addprefix %$(KERNEL_SUFFIX), _c.o _asm.o _cpp.o), $(ARLIBS))

include Make.rules
