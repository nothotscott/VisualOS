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

TARGET_NAME		:= $(notdir $(TARGET_DIR))

SOURCES			:= $(notdir $(wildcard $(THIS_NODE)/*.c $(THIS_NODE)/*.asm))
PRETARGETS		:= $(addprefix $(TARGET_DIR)/, $(addsuffix .o, $(subst .,_, $(SOURCES))))
TARGETS			:= $(TARGET_DIR)/$(TARGET_NAME).a
INCLUDE_DIRS	:= $(SUPER)/vos/include

# TODO -nostdinc
export CFLAGS	:= $(addprefix -I , $(INCLUDE_DIRS)) -fno-builtin -fvisibility=hidden -nostdlib -fno-builtin -g -fPIC -pedantic -Wall
ifndef DEBUG_MODE
#	CFLAGS += -O1
endif

LDFLAGS	:= -shared -nostdlib
ARLIBS	= $(addprefix $(TARGET_DIR)/, $(notdir $(wildcard $(TARGET_DIR)/*.o)))

all:	setup
		echo $(TARGET_DIR)
		+ $(MAKE) -f $(THIS)/Makefile pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	setup
setup:
		@mkdir -p $(TARGET_DIR)
		rm -f $(TARGETS)

.PHONY:	pretargets
pretargets:	$(PRETARGETS)


include Make.rules