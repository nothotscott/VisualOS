# This file's working directory is the root of the project

include Make.defaults

LDFLAGS	:= -T $(THIS)/kernel.ld -static -Bsymbolic -nostdlib

all:	$(OBJ_DIR)/kernel.elf

include Make.rules