# This Makefile's goal is to:
#  Compile VOS sys library glue (pretargets)
#  Call subdirectory libs
#  Relink/rearchive libs with the proper VOS sys lib glue (targets)

# This file's working directory is the root of the project
include Defaults.mk

# The names of kernel libraries that need an explicit kernel version at link time
KERNEL_LIBS			:= musl dlmalloc

SUBDIRS			:= $(filter-out $(THIS)/$(VOSLIB),$(shell find $(THIS)/* -maxdepth 0 -type d))

SOURCES					:= $(wildcard $(THIS)/*.c $(THIS)/*.cpp $(THIS)/*.asm) $(call find_many,$(THIS)/$(VOSLIB),c cpp asm)
PRETARGETS				:= $(patsubst $(THIS)/%.o,$(TARGET_DIR)/%.o,$(call to_obj,$(SOURCES)))
KERNEL_PRETARGETS		:= $(filter $(addprefix %$(KERNEL_SUFFIX),_c.o _asm.o _cpp.o),$(PRETARGETS))
USERSPACE_PRETARGETS	:= $(filter $(addprefix %$(USERSPACE_SUFFIX),_c.o _asm.o _cpp.o),$(PRETARGETS))
INDEPENDENT_PRETARGETS	:= $(filter-out $(USERSPACE_PRETARGETS) $(KERNEL_PRETARGETS),$(PRETARGETS))

INCLUDE_DIRS			:= $(THIS)/ $(THIS)/$(VOSLIB)/include
INCLUDE_LIB_DIRS		:= $(DEFAULT_LIBC_INCLUDES)

SUBDIR_ROS		= $(shell find $(LIBS_DIR)/ -mindepth 2 -name *.ro -type f)
SUBDIR_SPECIAL	= $(filter $(KERNEL_LIBS), $(basename $(notdir $(SUBDIR_ROS))))
TARGETS			= $(addprefix $(LIBS_DIR)/,																\
					$(addsuffix .a,$(filter-out $(SUBDIR_SPECIAL),$(basename $(notdir $(SUBDIR_ROS)))))	\
					$(addsuffix $(KERNEL_SUFFIX).ro,$(SUBDIR_SPECIAL))									\
					$(addsuffix $(USERSPACE_SUFFIX).a,$(SUBDIR_SPECIAL))								\
					$(VOSLIB).ro																		\
				)

CFLAGS	:= $(addprefix -I ,$(INCLUDE_DIRS)) $(addprefix -isystem , $(INCLUDE_LIB_DIRS)) $(DEFAULT_CFLAGS) $(DEBUG_CFLAGS)
CFLAGS	+= -Wall -Wextra -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wredundant-decls -Winline -Wuninitialized -Wnested-externs -Wdeclaration-after-statement
ifeq ($(CC), gcc)
	CFLAGS	+= -fvar-tracking
endif
ASFLAGS	:= $(DEFAULT_ASFLAGS) $(DEBUG_ASFLAGS)

LDLIBS	= $(INDEPENDENT_PRETARGETS)
ARLIBS	= $(INDEPENDENT_PRETARGETS)


all:	clean
		+ $(MAKE) -f $(THIS)/Makefile pretargets subdirs
		+ $(MAKE) -f $(THIS)/Makefile targets

.PHONY: 	$(SUBDIRS) subdirs
$(SUBDIRS):	
			+ $(MAKE) -f $@/Makefile SUPER=$(THIS) THIS=$@ TARGET_DIR=$(TARGET_DIR)/$(notdir $@)
subdirs:	$(SUBDIRS)
			@:

.PHONY:		pretargets
pretargets:	$(PRETARGETS)
			@:

.PHONY:		targets
targets:	$(TARGETS)
			@:

.PHONY:		clean
clean:
		$(if $(wildcard $(LIBS_DIR)),rm -f $(TARGETS),)


include Rules.mk

SOURCE_RO	= $(filter %/$*.ro,$(SUBDIR_ROS))

$(TARGET_DIR)/%$(KERNEL_SUFFIX).ro:
			$(LD) $(LDFLAGS) -r $(SOURCE_RO) -o $@

$(TARGET_DIR)/%$(KERNEL_SUFFIX).a:
			$(AR) $(ARFLAGS) $@ $(SOURCE_RO) $(ARLIBS) $(KERNEL_PRETARGETS)

$(TARGET_DIR)/%$(USERSPACE_SUFFIX).a:
			$(AR) $(ARFLAGS) $@ $(SOURCE_RO) $(ARLIBS) $(USERSPACE_PRETARGETS)

$(TARGET_DIR)/%.a:
			$(AR) $(ARFLAGS) $@ $(SOURCE_RO) $(ARLIBS)
