################################################################################
# This file is not native to PDClib and is a part of VisualOS
# Copyright 2020 Scott Maday
# Check the LICENSE file that came with VisualOS for licensing terms
################################################################################
# This file's working directory is the root of the project

include Defaults.mk


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

CFLAGS	:= $(addprefix -I , $(INCLUDE_DIRS)) $(DEFAULT_CFLAGS) $(DEBUG_CFLAGS) -pedantic -fvisibility=hidden -fdata-sections -ffunction-sections -D_STDC_PREDEF_H
CFLAGS	+= -Wall -Wextra -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Winline -Wno-long-long -Wuninitialized -Wnested-externs -Wdeclaration-after-statement
ASFLAGS	:= $(DEFAULT_ASFLAGS) $(DEBUG_ASFLAGS)

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
clean:
ifneq "$(wildcard $(TARGETS))" ""
		rm -f $(TARGETS)
endif

include Rules.mk