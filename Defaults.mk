## The following should be from the command line
OSNAME		:= VisualOS
BUILD_DIR	:= build
BIN_DIR		:= bin

# Build procedures
KERNEL		:= kernel
LIBS		:= libraries
LIBC		:= musl

# Explicit kernel linking directive
KERNEL_SUFFIX		:= _k
USERSPACE_SUFFIX	:= _u

# Directories
SRC_DIR			:= src
BOOTLOADER_DIR	:= $(BIN_DIR)/bootloader
KERNEL_DIR		:= $(BUILD_DIR)/$(KERNEL)
LIBS_DIR		:= $(BUILD_DIR)/$(LIBS)
ASMDUMP_DIR		:= $(BUILD_DIR)/asm

# Programs
CC		:= gcc
CXX		:= g++
LD		:= ld
AS		:= nasm
ML		:= ocamlopt
MAKE	:= make
OBJCOPY	:= objcopy
# LLVM
ifeq ($(TOOLCHAIN),llvm)
CC		:= clang
CXX		:= clang++
LD		:= lld
endif
CAS		:= $(CC)

# Flags
DEFAULT_CFLAGS	:= -fno-pic -fpie -m64 -ffreestanding -fno-builtin -nostdinc -fno-omit-frame-pointer
DEFAULT_ASFLAGS	:= -f elf64
ifdef DEBUG_MODE
	DEBUG_CFLAGS	:= -O0 -gdwarf
	DEBUG_ASFLAGS	:= -g -F dwarf
	ifeq ($(CC), gcc)
		DEBUG_CFLAGS	+= -fvar-tracking
	endif
else
	DEBUG_CFLAGS	:= -O1
	DEBUG_ASFLAGS	:= -g -F dwarf
endif

# Shell configurations
NORMAL	:= $(shell tput sgr0)
BOLD	:= $(shell tput bold)
RED		:= $(shell tput setaf 1)
GREEN	:= $(shell tput setaf 2)
YELLOW	:= $(shell tput setaf 3)
BLUE	:= $(shell tput setaf 4)
MAGENTA	:= $(shell tput setaf 5)
CYAN	:= $(shell tput setaf 6)

# Other stuff
BUILD_DIR_ABS	:= $(abspath $(BUILD_DIR))
# HOST			:= $(shell $(CC) -dumpmachine | sed "s/\(-\).*$$//")
# ARCH			:= $(shell $(CC) -dumpmachine | sed "s/\(-\).*$$//")
# LIBGCC		:= $(shell $(CC) -print-libgcc-file-name)

# Functions
define echo_build
	@echo "$(GREEN)$(BOLD)Building $@$(NORMAL)"
endef

define to_obj
	$(addsuffix .o, $(subst .,_, $(1)))
endef

define find_many
	$(foreach ext,$(2),								\
		$(shell find $(1) -type f -name '*.$(ext)')	\
	)
endef

# Rules

default:	all