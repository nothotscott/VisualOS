# This file's working directory is the root of the project

include Make.defaults

SOURCES	:= $(notdir $(wildcard $(THIS)/*.c $(THIS)/*.cpp $(THIS)/*.asm))
TARGETS	:= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SOURCES))))

CFLAGS	+= -I$(THIS)

all:	$(TARGETS)

include Make.rules