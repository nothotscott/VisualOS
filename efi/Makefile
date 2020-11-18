# This file's working directory is the root of the project

include Make.defaults

ARCH		:= x86_64
LIBDIR		:= /gnuefi

LDSCRIPT	:= $(THIS)/gnu-efi/elf_$(ARCH)_efi.lds
CRT0		:= $(shell find $(LIBDIR) -name crt0-efi-$(ARCH).o 2>/dev/null | tail -n1)
INCDIR		:= /usr/include

CFLAGS			= -I. -I$(INCDIR)/efi -I$(INCDIR)/efi/$(ARCH) -DEFI_FUNCTION_WRAPPER -fPIC -fshort-wchar -ffreestanding -Wall -Ifs/ -Iloaders/ -D$(ARCH) -Werror -mno-red-zone
LDFLAGS			= -T $(LDSCRIPT) -Bsymbolic -shared -nostdlib -znocombreloc -L$(LIBDIR) $(CRT0)
LOADLIBES		= -lgnuefi $(LIBGCC)
OBJCOPYFLAGS	= -S --target=efi-app-$(ARCH)

# .PRECIOUS: $(OBJ_DIR)/main.so
all:	$(BUILD_DIR)/main.efi

include Make.rules