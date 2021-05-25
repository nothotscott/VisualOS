# This file's working directory is the root of the project

include Defaults.mk

SOURCES				:= $(call find_many,$(THIS),c cpp asm)
PRETARGETS			:= $(patsubst $(THIS)/%.o,$(TARGET_DIR)/%.o,$(call to_obj,$(SOURCES)))
TARGETS				:= $(foreach target, $(patsubst $(THIS)/%,%,$(wildcard $(THIS)/*/*)),	\
							$(TARGET_DIR)/$(target)/$(notdir $(target)).ro					\
						)

INCLUDE_DIRS		:= $(THIS)/
INCLUDE_LIB_DIRS	:= $(DEFAULT_LIBC_INCLUDES)

CFLAGS	:= $(addprefix -I ,$(INCLUDE_DIRS)) $(addprefix -isystem , $(INCLUDE_LIB_DIRS)) $(DEFAULT_CFLAGS) $(DEBUG_CFLAGS) -std=c11
ASFLAGS	:= $(addprefix -i ,$(INCLUDE_DIRS)) $(DEFAULT_ASFLAGS) $(DEBUG_ASFLAGS)
LDFLAGS	:= -nostdlib
LDLIBS	= $(PRETARGETS)

all:	clean
		+ $(MAKE) -f $(THIS)/Makefile pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	pretargets
pretargets:	$(PRETARGETS)

.PHONY:		clean
clean:
			$(if $(wildcard $(LIBS_DIR)),rm -f $(TARGETS),)


include Rules.mk


PRETARGET_OBJS	= $(filter $(TARGET_DIR)/$(dir $*)%,$(PRETARGETS))

$(TARGET_DIR)/%.ro:
			$(LD) $(LDFLAGS) -r $(PRETARGET_OBJS) -o $@