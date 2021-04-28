# This Makefile's goal is to:
#  Compile VOS sys library glue (pretargets)
#  Call subdirectory libs
#  Relink/rearchive libs with the proper VOS sys lib glue (targets)

# This file's working directory is the root of the project

include Make.defaults

# The names of kernel libraries that need an explicit kernel version at link time
KERNEL_LIBS			:= pdclib
# Only used in this context for explicit userspace use
USERSPACE_SUFFIX	:= _u

SUBDIRS			:= $(shell find $(THIS)/* -maxdepth 0 -type d)

SOURCES					:= $(notdir $(wildcard $(THIS)/*.c $(THIS)/*.cpp $(THIS)/*.asm))
PRETARGETS				:= $(addprefix $(LIBS_DIR)/,$(call to_obj,$(SOURCES)))
USERSPACE_PRETARGETS	:= $(filter $(addprefix %$(USERSPACE_SUFFIX),_c.o _asm.o _cpp.o),$(PRETARGETS))
KERNEL_PRETARGETS		:= $(filter $(addprefix %$(KERNEL_SUFFIX),_c.o _asm.o _cpp.o),$(PRETARGETS))
INDEPENDENT_PRETARGETS	:= $(filter-out $(USERSPACE_PRETARGETS) $(KERNEL_PRETARGETS),$(PRETARGETS))
INCLUDE_DIRS			:= $(THIS)/ $(THIS)/$(LIBC)/include $(THIS)/$(LIBC)/vos/include

SUBDIR_LIBS		= $(shell find $(LIBS_DIR)/ -mindepth 2 -name *.ro -type f)
TARGETS			= $(addprefix $(LIBS_DIR)/,																			\
					$(addsuffix .a,$(basename $(notdir $(SUBDIR_LIBS))))											\
					$(addsuffix $(KERNEL_SUFFIX).a,$(filter $(KERNEL_LIBS), $(basename $(notdir $(SUBDIR_LIBS)))))	\
				)

export CFLAGS	:= $(addprefix -I , $(INCLUDE_DIRS)) -fno-builtin -fvisibility=hidden -fno-builtin -fshort-wchar -std=c11 -nostdlib -gdwarf -fvar-tracking -O0 -fno-pic -fpie -nostdinc -pedantic
CFLAGS			+= -Wall -Wextra -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Winline -Wno-long-long -Wuninitialized -Wnested-externs -Wdeclaration-after-statement
ifndef DEBUG_MODE
	CFLAGS += -O1
endif
#export ASFLAGS	:= 

ARLIBS	= $(INDEPENDENT_PRETARGETS)


all:	
		+ $(MAKE) -f $(THIS)/Makefile pretargets subdirs
		+ $(MAKE) -f $(THIS)/Makefile targets

.PHONY: 	$(SUBDIRS) subdirs
$(SUBDIRS):	
		+ $(MAKE) -f $@/Makefile SUPER=$(THIS) THIS=$@ THIS_NODE=$@ TARGET_DIR=$(TARGET_DIR)/$(notdir $@)
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


include Make.rules

$(TARGET_DIR)/%$(KERNEL_SUFFIX).a:
			$(AR) $(ARFLAGS) $@ $(filter %/$*.ro,$(SUBDIR_LIBS)) $(ARLIBS) $(KERNEL_PRETARGETS)

$(TARGET_DIR)/%.a:
			$(AR) $(ARFLAGS) $@ $(filter %/$*.ro,$(SUBDIR_LIBS)) $(ARLIBS) $(USERSPACE_PRETARGETS)
