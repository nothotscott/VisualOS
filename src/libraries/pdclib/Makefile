################################################################################
# This file is not native to PDClib and is a part of VisualOS
# Copyright 2020 Scott Maday
# Check the LICENSE file that came with VisualOS for licensing terms
################################################################################
# This file's working directory is the root of the project

include Make.defaults


ROOT_SUBDIRS	:= functions vos
RECURSIVE_DIRS	:= $(patsubst %/Makefile,%, $(shell find $(THIS)/ -mindepth 2 -type f -name 'Makefile'))

SOURCES			:= $(patsubst $(THIS)/%,%,																				\
					$(patsubst $(addsuffix %, $(RECURSIVE_DIRS)),, 														\
						$(foreach root_subdir, $(ROOT_SUBDIRS), $(call find_many, $(THIS)/$(root_subdir),c cpp asm))	\
					)																									\
				)
PRETARGETS		:= $(addprefix $(TARGET_DIR)/, $(call to_obj,$(SOURCES)))
TARGETS			:= $(TARGET_DIR)/$(LIBC).ro
INCLUDE_DIRS	:= $(THIS)/include $(THIS)/vos/include $(SRC_DIR)/$(LIBS)

# TODO remove -D__STDC_NO_THREADS__ (might have to remove -pedantic for syscalls with 0 args)
export CFLAGS	:= $(addprefix -I , $(INCLUDE_DIRS)) -std=c11 -nostdlib -gdwarf -fvar-tracking -O0 -fno-pic -fpie -nostdinc -pedantic -D_STDC_PREDEF_H -D__STDC_NO_THREADS__
CFLAGS			+= -fno-builtin -fvisibility=hidden -fshort-wchar -fdata-sections -ffunction-sections 
CFLAGS			+= -Wall -Wextra -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Winline -Wno-long-long -Wuninitialized
CFLAGS			+= -Wnested-externs -Wdeclaration-after-statement
ifndef DEBUG_MODE
	CFLAGS += -O1
endif
# export ASFLAGS	:= 

LDLIBS	= $(PRETARGETS)

all:	clean
		+ $(MAKE) -f $(THIS)/Makefile pretargets recursive_dirs
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)


.PHONY: 	$(RECURSIVE_DIRS) recursive_dirs
$(RECURSIVE_DIRS):	
			+ $(MAKE) -f $@/Makefile SUPER=$(THIS) THIS=$@ TARGET_DIR=$(patsubst $(THIS)/%,$(TARGET_DIR)/%, $@)
recursive_dirs:	$(RECURSIVE_DIRS)
			@:

.PHONY:		pretargets
pretargets:	$(PRETARGETS)
			@:

.PHONY:		clean
ifneq "$(wildcard $(TARGETS))" ""
		rm -f $(TARGETS)
endif

include Make.rules
