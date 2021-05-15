################################################################################
# This file is not native to musl and is a part of VisualOS
# Copyright 2021 Scott Maday
# Check the LICENSE file that came with VisualOS for licensing terms
################################################################################
# This file's working directory is the root of the project

include Defaults.mk

ARCH	:= x86_64

SOURCES_RAW		:= $(call find_many,$(THIS),c cpp asm s)
SOURCES			:= $(filter-out $(foreach source,$(SOURCES_RAW),												\
						$(if $(wildcard $(dir $(source))$(ARCH)/$(basename $(notdir $(source))).*),$(source),)	\
					), $(SOURCES_RAW))
PRETARGETS		:= $(patsubst $(THIS)/%.o,$(TARGET_DIR)/%.o,$(call to_obj,$(SOURCES)))
TARGETS			:= $(TARGET_DIR)/$(LIBC).ro
INCLUDE_DIRS	:= $(SUPER)/ $(THIS)/arch/$(ARCH) $(THIS)/arch/generic $(THIS)/obj/src/internal $(THIS)/src/include $(THIS)/src/internal $(THIS)/obj/include $(THIS)/include

CFLAGS		:= $(addprefix -I ,$(INCLUDE_DIRS)) $(DEFAULT_CFLAGS) $(DEBUG_CFLAGS) -fvisibility=default -fdata-sections -ffunction-sections
CFLAGS		+= -std=c99 -pipe -D_XOPEN_SOURCE=700
CASFLAGS	:= $(CFLAGS)
LDFLAGS		:= -nostdlib
LDLIBS		= $(PRETARGETS)

all:	clean
		+ $(MAKE) -f $(THIS)/Makefile pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	pretargets
pretargets:	$(PRETARGETS)

.PHONY:		clean
clean:
ifneq "$(wildcard $(TARGETS))" ""
		rm -f $(TARGETS)
endif


include Rules.mk