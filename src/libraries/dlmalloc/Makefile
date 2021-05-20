# Copyright 2021 Scott Maday
# Check the license provided with VisualOS

# This file's working directory is the root of the project
include Defaults.mk

SOURCES				:= $(call find_many,$(THIS),c cpp asm)
PRETARGETS			:= $(patsubst $(THIS)/%.o,$(TARGET_DIR)/%.o,$(call to_obj,$(SOURCES)))
TARGETS				:= $(TARGET_DIR)/$(notdir $(THIS)).ro

INCLUDE_LIB_DIRS	:= $(DEFAULT_LIBC_INCLUDES)
INCLUDE_DIRS		:= $(SUPER)/

PREPROCESSOR_DEFS	:=	HAVE_MORECORE=0			\
						MORECORE_CANNOT_TRIM	\
						LACKS_FCNTL_H			\
						LACKS_UNISTD			\
						LACKS_TIME_H=1			\

CFLAGS	:= $(addprefix -I , $(INCLUDE_DIRS)) $(addprefix -isystem , $(INCLUDE_LIB_DIRS)) $(addprefix -D , $(PREPROCESSOR_DEFS)) $(DEFAULT_CFLAGS) $(DEBUG_CFLAGS) $(FRAGMENT_CFLAGS)
CFLAGS	+= -Wall -Wextra -Werror -Wno-expansion-to-defined

LDLIBS	= $(PRETARGETS)

all:	clean
		+ $(MAKE) -f $(THIS)/Makefile pretargets
		+ $(MAKE) -f $(THIS)/Makefile targets

.PHONY:		pretargets
pretargets:	$(PRETARGETS)
			@:

.PHONY:		targets
targets:	$(TARGETS)
			@:

clean:
		$(if $(wildcard $(TARGET_DIR)),rm -f $(TARGETS),)

include Rules.mk