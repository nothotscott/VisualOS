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

SOURCES				:= $(notdir $(wildcard $(THIS_NODE)/*.c $(THIS_NODE)/*.asm))
PRETARGETS			:= $(addprefix $(TARGET_DIR)/, $(addsuffix .o, $(subst .,_, $(SOURCES))))
TARGETS				:= $(TARGET_DIR)/$(TARGET_NAME).a
INCLUDE_DIRS		:= $(SUPER)/vos/include
INCLUDE_LATER_DIRS	:= $(SUPER)/include

# TODO -nostdinc
export CFLAGS	:= $(addprefix -I , $(INCLUDE_LATER_DIRS)) -I- $(addprefix -I , $(INCLUDE_DIRS)) -fno-builtin -fvisibility=hidden -nostdlib -fno-builtin -g -DDEBUG -fPIC -pedantic -Wall -DUSE_SPIN_LOCKS=0 -DLACKS_SCHED_H
#CFLAGS			+= -DLACKS_SYS_TYPES_H -DLACKS_SYS_MMAN_H -DLACKS_FCNTL_H -DLACKS_UNISTD_H -DLACKS_SYS_PARAM_H
ifndef DEBUG_MODE
#	CFLAGS += -O1
endif

LDFLAGS	:= -shared -nostdlib
ARLIBS	= $(addprefix $(TARGET_DIR)/, $(notdir $(wildcard $(TARGET_DIR)/*.o)))

all:	setup
		+ $(MAKE) -f $(THIS)/Makefile pretargets
		+ $(MAKE) -f $(THIS)/Makefile $(TARGETS)

.PHONY:	setup
setup:
		@mkdir -p $(TARGET_DIR)
		rm -f $(TARGETS)

.PHONY:	pretargets
pretargets:	$(PRETARGETS)


include Make.rules